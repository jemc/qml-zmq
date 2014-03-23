
#ifndef QML_ZMQ_UTIL_H
#define QML_ZMQ_UTIL_H

#include <QtQml>


class ZMQ_Util : public QObject
{
    Q_OBJECT
    
public:
    static QObject* qmlAttachedProperties(QObject* object);
    
    static inline const char* dataChars()
    { return " !\"#$&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; };
    
    static inline QString convertBytesToData(const QByteArray& bytes)
    { return QString::fromLatin1(bytes.toPercentEncoding(dataChars())); }
    
    static inline QByteArray convertDataToBytes(const QString& str)
    { return QByteArray::fromPercentEncoding(str.toLatin1()); }
};


class ZMQ_UtilAttached : public QObject
{
    Q_OBJECT
    
public:
    ZMQ_UtilAttached(QObject* attached)
    { m_attached = attached; };
    
private:
    QObject* m_attached = NULL;
    
public slots:
    QString convertLatin1ToData(const QString& str)
    { return ZMQ_Util::convertBytesToData(str.toLatin1()); }
    
    QString convertDataToLatin1(const QString& data)
    { return QString::fromLatin1(ZMQ_Util::convertDataToBytes(data)); }
};


QML_DECLARE_TYPEINFO(ZMQ_Util, QML_HAS_ATTACHED_PROPERTIES)

#endif
