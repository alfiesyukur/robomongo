#pragma once

#include <QObject>
#include <QMutex>
#include <boost/scoped_ptr.hpp>
#include <mongo/scripting/engine.h>
#include <js/jsparse.h>

#include "robomongo/core/engine/Result.h"

namespace Robomongo
{
    class ConnectionSettings;

    class ScriptEngine : public QObject
    {
        Q_OBJECT
    public:
        ScriptEngine(ConnectionSettings *connection);
        ~ScriptEngine();

        void init();
        ExecResult exec(const QString &script, const QString &dbName = QString());

        void use(const QString &dbName);

    private:
        ConnectionSettings *_connection;
        QString _currentDatabase;
        bool _isCurrentDatabaseValid;

        Result prepareResult(const QString &output, const QList<mongo::BSONObj> objects, qint64 elapsedms);
        ExecResult prepareExecResult(const QList<Result> &results);

        QString getString(const char *fieldName);

        bool statementize(const QString &script, QStringList &outList, QString &outError);
        QStringList statementize2(const QString &script);
        void parseTree(JSParseNode * root, int indent, const QString &script, QStringList &list, bool topList);


        // Order is important. mongo::Scope should be destroyed before mongo::ScriptEngine
        boost::scoped_ptr<mongo::ScriptEngine> _engine;
        boost::scoped_ptr<mongo::Scope> _scope;

        QString subb(const QStringList &list, int fline, int fpos, int tline, int tpos);

        QMutex _mutex;


    };
}