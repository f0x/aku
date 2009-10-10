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
const QString overwriteRequest = "already exists. Overwrite it ?";

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
    Q_UNUSED(args)

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

void RarPlugin::init(const KUrl &fileName, const QString &password)
{
    m_fileName = fileName;
    m_password = password;
}

void RarPlugin::loadArchive()
{
    AkuData akudata;

    QByteArray output;
    QString outputCodec;
    QStringList options;

    // v[t,b]        Verbosely list archive [technical,bare]
    // -p-            Do not query password
    options << "vt" << "-p-";

    if (!m_password.isEmpty()) {
        options << "-p" + m_password;
        kDebug() << options;
    }

    options << m_fileName.pathOrUrl();

    // we start a first archive list to check if the archive is locked
    // or header password protected
    QProcess *localProcess = new QProcess;
    localProcess->setProcessChannelMode(QProcess::MergedChannels);
    localProcess->start(exeName, options);
    localProcess->waitForFinished(-1);

    output = localProcess->readAllStandardOutput();
    outputCodec = QString::fromLocal8Bit(output);

    if (outputCodec.contains("CRC failed in")) {
        kDebug() << "The archive is HEADER PROTECTED";
        akudata.headerprotected = true;
        onArchiveLoaded(akudata);
        return;
    } else if (outputCodec.contains("Lock is present")) {
        kDebug() << "The archive is LOCKED";
    }

    options.clear();

    // we start the real archive list
    options << "v";

    if (!m_password.isEmpty()) {
        options << "-p" + m_password;
        kDebug() << options;
    }

    options << m_fileName.pathOrUrl();

    QProcess *process = new QProcess;
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
    process->start(exeName, options);
    process->waitForFinished(-1);

    output = process->readAllStandardOutput();
    outputCodec = QString::fromLocal8Bit(output);
    kDebug() << "NEW OUTPUT " + output;

    if (outputCodec.contains("Comment:")) {
        QString comment = output;
        int target = comment.indexOf("Comment:");
        comment.remove(0, target);
        comment.remove("Comment: ");
        comment.remove(comment.indexOf("Pathname"), comment.length());
        comment = comment.trimmed();
        akudata.comment = comment;
        kDebug() << "COMMENT: " << comment;
    }

    int indexOfHeaderLine;
    indexOfHeaderLine = outputCodec.indexOf(headerLine);

    // cut the the text until the end of headerLine
    outputCodec.remove(0, indexOfHeaderLine + 79);

    // search for the second headerLine. The list of the file in the archive ends here
    indexOfHeaderLine = outputCodec.indexOf(headerLine);

    outputCodec.remove(indexOfHeaderLine, output.length());

    QTextStream stream(&outputCodec);
    QString line;
    QStringList file;
    QStringList filespasswordprotected;
    int i = 0;

    do {
        line = stream.readLine();
        // skip the empty line
        if (line.isEmpty()) {
            continue;
        }

        if ((i % 2) == 0) {
            if (line.startsWith('*') && (m_password.isEmpty())) {
                filespasswordprotected << line.mid(1);
                file << line.mid(1) + " *";
            } else {
                file << line.mid(1); // filepath
            }
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
            akudata.paths << (QStringList() << file);
            akudata.passwordprotectedPaths << (QStringList() << filespasswordprotected);
            file.clear();
        }
        i++;
    } while (!line.isNull());

    onArchiveLoaded(akudata);
}

QStringList RarPlugin::additionalHeaderStrings()
{
    return QStringList() << i18n("Ratio") << i18n("Modified")
                         << i18n("Attributes") << i18n("CRC")
                         << i18n("Method") << i18n("Version");
}

void RarPlugin::extractArchive(const AkuExtractInfo &extractInfo, const AkuPlugin::ExtractionOptions &extractionOptions)
{
//    Usage:     unrar <command> -<switch 1> -<switch N> <archive> <files...>
//               <@listfiles...> <path_to_extract\> 
    QStringList options;
    if (extractionOptions & AkuPlugin::ExctractFullPaths) {
        options << "x";   // x     Extract files with full path
    }
    else {
        options << "e";  //  e     Extract files to current directory
    }

    if (extractionOptions & AkuPlugin::AskBeforeOverwrite) {
        // do nothing. It's a standard option
    }
    if (extractionOptions & AkuPlugin::OverwriteWithoutPrompt) {
        options << "-o+";  //  o[+|-]        Set the overwrite mode
    }
    if (extractionOptions & AkuPlugin::SkipExistingFiles) {
        options << "-o-";  //  o[+|-]        Set the overwrite mode
    }
    if (extractionOptions & AkuPlugin::RenameAutomatically) {
        options << "-or"; //  or     Rename files automatically
    }

    if (extractionOptions & AkuPlugin::OpenDestinationPath) {
    }

    if (extractInfo.headerpassword.isEmpty()) {
        options << "-p-";    // p-     Do not query password
    } else {
        options << "-p" + extractInfo.headerpassword;
    }

    options << m_fileName.pathOrUrl();

    options << extractInfo.files;

    options << extractInfo.destination.pathOrUrl();

    //options <<

    kDebug() << options;

    m_process = new QProcess;
    connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
    //connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(getError()));
    m_process->start(exeName, options);
    m_process->waitForFinished(-1);
    //kDebug() << m_process->readAllStandardOutput();
}

void RarPlugin::addToArchive(const QStringList &files, const QString &path)
{
    QProcess *process = new QProcess;
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(getError()));

    QStringList options;
    options << "a";   // a    Add files to archive
    options << "-ep1";  // ep1  Exclude base directory from names or "ep" (verify)
    if (!path.isEmpty()) {
        options << "-ap" + path;     // ap<path> Set path inside archive
    }

    if (!m_password.isEmpty()) {
        options << "-p" + m_password;
    }

    options << m_fileName.pathOrUrl();

    kDebug() << options;

    for (int i = 0; i < files.size(); ++i) {
        options << files[i];
    }

    process->start(exeName, options);
    process->waitForFinished(-1);
}

void RarPlugin::removeFromArchive(const QStringList &files)
{
}

void RarPlugin::lockArchive()
{  
    kDebug() << "locking archive";
    QStringList options;

    options << "k";     //  k    Lock the archive

    if (!m_password.isEmpty()) {
        options << "-p" + m_password;
    }

    options << m_fileName.pathOrUrl();

    QProcess *process = new QProcess;
    connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
    process->start(exeName, options);
    process->waitForFinished();
}

void RarPlugin::getError() {
    QProcess *sender = qobject_cast<QProcess *>(this->sender());
    QByteArray error = sender->readAllStandardError();

    QString errorCodec = QString::fromLocal8Bit(error);
    kDebug() << "ERRORE: " + errorCodec;
    if (errorCodec.isEmpty()) {
        return;
    }

    // overwrite request
    if (errorCodec.contains(overwriteRequest)) {
        QTextStream stream(&errorCodec);
        QString line;
        do {
            line = stream.readLine();
            if (line.contains(overwriteRequest)) {
                int index = line.indexOf(overwriteRequest);
                line.remove(index - 1, line.length());
                // line is the name of the file
                onError(AkuPlugin::RequestOverwrite, line);
                return;
            }

        } while (!line.isNull());

        return;
    }
    //
    onError(AkuPlugin::NormalError, errorCodec);
}

void RarPlugin::setAnswer(OverwriteAnswer answer, const QString &info)
{
    switch (answer) {
            case (Yes):
                kDebug() << QString::number(m_process->write("y\n"));
                break;
            case (No):
                kDebug() << QString::number(m_process->write("n\n"));
                break;
            case (Rename):
                break;
            case (Quit):
                kDebug() << QString::number(m_process->write("q\n"));
                kDebug() << m_process->readAllStandardError();
                break;
            default: ;
    }

}

