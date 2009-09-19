/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
 *                     Alessandro Diaferia <alediaferia@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "rarplugin.h"

#include <QProcess>
#include <QDir>
#include <QTextStream>

#include <KLocale>
#include <KStandardDirs>
#include <KDateTime>
#include <KDebug>

AKU_PLUGIN_EXPORT(RarPlugin)

// the list of files in the rar archive starts after this line
const QString headerLine = "-------------------------------------------------------------------------------";

// name of the executable
QString exeName;

bool rarCreate = false;
bool rarRename = false;
bool rarDelete = false;
bool rarAddComment = false;
bool rarEncrypt = false;
bool rarLock = false;
bool rarAdd = false;

RarPlugin::RarPlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent)
{
    if (!KStandardDirs::findExe("rar").isEmpty()) {
        rarCreate = true;
        rarRename = true;
        rarDelete = true;
        rarAddComment = true;
        rarEncrypt = true;
        rarLock = true;
        rarAdd = true;
    }
}

RarPlugin::~RarPlugin()
{
}


bool RarPlugin::isInstalled()
{
    // WARNING: this checks for unrar or rar with no distinction
    // TODO: decide whether we should support also the shareware rar

    if (KStandardDirs::findExe("rar").isEmpty()) {
        if (!KStandardDirs::findExe("unrar").isEmpty()) {
            exeName = "unrar";
            return true;
        }
        return false;
    }
    else {
        exeName = "rar";
        return true;
    }
}

QStringList RarPlugin::mimeTypeNames()
{
    return QStringList() << "application/x-rar";
}

bool RarPlugin::canExtract()
{
    return true;
}

bool RarPlugin::canAdd()
{
    return rarAdd;
}

bool RarPlugin::canCreate()
{
    return rarCreate;
}

bool RarPlugin::canRename()
{
    return rarRename;
}

bool RarPlugin::canDelete()
{
    return rarDelete;
}

bool RarPlugin::canAddComment()
{
    return rarAddComment;
}

bool RarPlugin::canEncrypt()
{
    return rarEncrypt;
}

bool RarPlugin::canLock()
{
    return rarLock;
}

void RarPlugin::init(const KUrl &fileName)
{
    m_fileName = fileName;
}

void RarPlugin::loadArchive()
{
    // we start a first archive list to check if the archive is locked
    // or header password protected
    QProcess process;
    QString output;
    QStringList options;

    // v[t,b]        Verbosely list archive [technical,bare]
    // -p-            Do not query password
    options << "vt" << "-p-" << m_fileName.pathOrUrl();

    process.start(exeName, options);
    process.waitForFinished();

    output = process.readAllStandardOutput();

    if (output.contains("Lock is present")) {
        kDebug() << "The archive is LOCKED";
    } else if (output.contains("CRC failed in")) {
        kDebug() << "The archive is HEADER PROTECTED";
    }

    options.clear();

    // we start the real archive list
    options << "v" << m_fileName.pathOrUrl();

    process.start(exeName, options);
    process.waitForFinished();

    output = process.readAllStandardOutput();

    if (output.contains("Comment:")) {
        QString comment = output;
        int target = comment.indexOf("Comment:");
        comment.remove(0, target);
        comment.remove("Comment: ");
        comment.remove(comment.indexOf("Pathname"), comment.length());
        comment = comment.trimmed();
        kDebug() << "COMMENT: " << comment;
    }

    int indexOfHeaderLine;
    indexOfHeaderLine = output.indexOf(headerLine);

    // cut the the text until the end of headerLine
    output.remove(0, indexOfHeaderLine + 79);

    // search for the second headerLine. The list of the file in the archive ends here
    indexOfHeaderLine = output.indexOf(headerLine);

    output.remove(indexOfHeaderLine, output.length());

    QTextStream stream(&output);
    QString line;
    QVector<QStringList> archive;
    QStringList file;
    int i = 0;

    do {
        line = stream.readLine();
        // skip the empty line
        if (line.isEmpty()) {
            continue;
        }

        if ((i % 2) == 0) {
            file << line.mid(1); // filepath
        } else {
            QStringList attributes = (line.split(" ", QString::SkipEmptyParts));
            for (int i = 0; i < attributes.size(); i++) {
                if (i == 4) {
                    KDateTime modified(QDate::fromString(file[4], QString("dd-MM-yy")),
                                       QTime::fromString(attributes[4], QString("hh:mm")));
                    modified = modified.addYears(100);
                    file[4] = KGlobal::locale()->formatDateTime(modified);
                    continue;
                }

                if (i == 0 || i == 1) {
                    file << attributes[i];
                    continue;
                }

                // ratio value.
                // The ratio value is inverse in rar output
                if (i == 2) {
                    // remove "%" at the end of the string
                    attributes[i].chop(1);
                    int value  = attributes[i].toInt();
                    value = 100 - value;
                    file << QString::number(value) + "%";
                    continue;
                }

                if (i > 3) {
                    file << attributes[i];
                    continue;
                }

                file << attributes[i];
            }
            archive << (QStringList() << file);
            file.clear();
        }
        i++;
    } while (!line.isNull());

    onArchiveLoaded(archive);
}

QStringList RarPlugin::additionalHeaderStrings()
{
    return QStringList() << i18n("Ratio") << i18n("Modified")
                         << i18n("Attributes") << i18n("CRC")
                         << i18n("Method") << i18n("Version");
}

void RarPlugin::extractArchive(const KUrl &destination, const QStringList &files)
{
//    Usage:     unrar <command> -<switch 1> -<switch N> <archive> <files...>
//               <@listfiles...> <path_to_extract\> 
    QProcess process;
    QStringList options;
    options = files;
    options.insert(options.size(), destination.pathOrUrl());
    options.insert(0, "x");
    options.insert(1, m_fileName.pathOrUrl());
    process.start(exeName, options);
    kDebug() << options;
    process.waitForFinished();
}

void RarPlugin::lockArchive()
{  
    kDebug() << "locking archive";
    QProcess process;
    QStringList options;
    options.insert(0, "k");
    options.insert(1, m_fileName.pathOrUrl());
    process.start(exeName, options);
    kDebug() << options;
    process.waitForFinished();
}
