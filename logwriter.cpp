#include "logwriter.h"

LogWriter::LogWriter(QString filename) :
    myFile(filename)
{
    myFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
}
void LogWriter::log(QString str)
{
    QTextStream out(&myFile);
    QString time = QTime().currentTime().toString("hh:mm:ss.zzz");
    out << "[" << time << "] " << str;
}
LogWriter::~LogWriter()
{
    myFile.close();
    //delete myFile;
}
