/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
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

#include "szipplugin.h"

#include <QProcess>
#include <QDateTime>
// #include <QFile>

#include <KLocale>
#include <KDebug>
#include <KStandardDirs>

AKU_PLUGIN_EXPORT(SzipPlugin)

// the list of files in the 7zip archive starts after this line
const QString headerLine = "----------";

// name of the executable
QString exeName;


SzipPlugin::SzipPlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent)
{}

SzipPlugin::~SzipPlugin()
{}

QStringList SzipPlugin::mimeTypeNames()
{
    return QStringList() << "application/x-7z-compressed";
}

bool SzipPlugin::canExtract()
{
    return true;
}

bool SzipPlugin::canAdd()
{
    return true;
}

bool SzipPlugin::canCreate()
{
    return false;
}

bool SzipPlugin::canRename()
{
    return true;
}

bool SzipPlugin::canDelete()
{
    return true;
}

bool SzipPlugin::canEncrypt()
{
    return true;
}

bool SzipPlugin::canAddComment()
{
    return true;
}

bool SzipPlugin::canLock()
{
    return false;
}


void SzipPlugin::loadArchive()
{
    QProcess *process = new QProcess();
   
    QStringList options;
    options << "l" << "-slt" << m_fileName.pathOrUrl();
    process->start(exeName, options);
    process->waitForFinished();
    
    QString output;
    output = process->readAllStandardOutput();
   
    int indexOfHeaderLine;
    indexOfHeaderLine = output.indexOf(headerLine);
    output.remove(0, indexOfHeaderLine+10);
    output.trimmed();
    output = output.trimmed();

    float ratio;
    QString ratioValue;   
    QString packed;
    QString size;

    QTextStream stream(&output);
    QString line;
    QVector<QStringList> archive;
    QStringList file;

    do {
        line = stream.readLine();
        if (line.startsWith("Path =")) {
           file << line.mid(7);
           //kDebug() << file;
           continue;
        }

        if (line.startsWith("Size =")) {
           //file << line.mid(7);
           size = line.mid(7);
           file << KGlobal::locale()->formatByteSize(size.toDouble());
           //kDebug() << file;
           continue;
       }
       if (line.startsWith("Packed Size =")) {
           packed = line.mid(14);
           file << KGlobal::locale()->formatByteSize(packed.toDouble());
           //file << line.mid(14);
           // add the ratio value
           if (size.toInt() != 0) {
               ratio = 100 * packed.toInt() / size.toInt();
               ratioValue.setNum(ratio);
               file << (ratioValue + "%");
               continue;
           }
           file << QString("0%");
           //kDebug() << file;
           continue;
       }
       if (line.startsWith("Modified =")) {
           //kDebug() << line.mid(11,10);
           //kDebug() << line.mid(22,5);
           QDateTime modified(QDate::fromString(line.mid(11, 10), QString("yyyy-MM-dd")),
                              QTime::fromString(line.mid(22, 5), QString("hh:mm")));
           file << KGlobal::locale()->formatDateTime(modified);
           //kDebug() << file;
           continue;
       }
       if (line.startsWith("Attributes =")) {
           file << line.mid(13);
           //kDebug() << file;
           continue;
       }
       if (line.startsWith("CRC =")) {
           file << line.mid(6);
           //kDebug() << file;
           continue;
       }
       if (line.startsWith("Method =")) {
           file << line.mid(9);
           //kDebug() << file;
           continue;
       }
       if (line.startsWith("Block =")) {
           file << line.mid(8);
           //kDebug() << file;
           archive << file;
           file.clear();
       }

    } while (!line.isNull());

    onArchiveLoaded(archive);
}

bool SzipPlugin::isInstalled()
{
    if (!KStandardDirs::findExe("7z").isEmpty()) {
        exeName = "7z";
        return true;
    }
    return false;
}

void SzipPlugin::init(const KUrl &fileName)
{
    m_fileName = fileName;
}

QStringList SzipPlugin::additionalHeaderStrings()
{
    return QStringList() << i18n("Ratio") << i18n("Modified") << i18n("Attributes") << i18n("CRC") << i18n("Method") << i18n("Block");
}

void SzipPlugin::extractArchive(const KUrl &destination, const QStringList &files)
{
//  Usage: 7z <command> [<switches>...] <archive_name> [<file_names>...]
//            [<@listfiles...>] 
    QProcess process;
    QStringList options;
    options = files;
    options.insert(0, "x");
    options.insert(1, "-o" + destination.pathOrUrl());
    options.insert(3, m_fileName.pathOrUrl());
    process.start(exeName, options);
    kDebug() << exeName;
    kDebug() << options;
    process.waitForFinished();
}
