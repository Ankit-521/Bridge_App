#pragma once

#include <QDialog>
class QStringListModel;
class QGridLayout;

namespace Ui {
class ChatForm;
}

class ChatForm : public QDialog
{
    Q_OBJECT

public:
    explicit ChatForm(QWidget *parent = nullptr);
    ~ChatForm();

    void addTables(int tables);

public slots:
    void incomingMessageFromClient(const QString message, const int table, const QString sender);

private slots:
    void on_buttonSend_clicked();

    void on_lineEditMessage_returnPressed();

    void on_boxTypeMessage_currentIndexChanged(int index);

signals:
    void sendMessage(const QString &message, const QVector<int> tables = QVector<int>(), const QString &playerPair=0) const;

private:
    void prepareMessage();
    void enableBoxPartners(const bool enable);
    void addMessageToListView(const QString &message);

    Ui::ChatForm *ui;
    QStringListModel *m_model {nullptr};
    QGridLayout *m_gridLayout {nullptr};
};
