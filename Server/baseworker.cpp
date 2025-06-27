#include "baseworker.h"
QMap<QString,A> BaseWorker::socketmap;
QMap<int,QQueue<FileTransferContext>*>BaseWorker::queuemap;
QMap<int,QMutex*>BaseWorker::mutexmap;
QMap<int,QString>BaseWorker::idmarkmap;
QMutex BaseWorker::mutexmutex;
QMutex BaseWorker::idmarkmutex;
QMutex BaseWorker::socketmapmutex;
QMutex BaseWorker::queuemapmutex;
qint64 BaseWorker::m_fileid=0;
QMutex BaseWorker::fileidmutex;
QMap<int,B*>BaseWorker::offlinemsgmap;
QMap<int,bool>BaseWorker::vismap;
QMutex BaseWorker::vismapmutex;
QMutex BaseWorker::offlinemsgmapmutex;
BaseWorker::BaseWorker()
{

}


