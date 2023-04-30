#include "chatform.h"
#include "ui_chatform.h"

#include <QStringListModel>
#include <QRadioButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QTime>
#include <QMessageBox>
#include <QDebug>

ChatForm::ChatForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatForm)
{
    ui->setupUi(this);

    m_model = new QStringListModel(this);
    m_gridLayout = new QGridLayout(this);

    ui->gbListTable->setLayout(m_gridLayout);

    ui->boxTypeMessage->addItem("Global");
    ui->boxTypeMessage->addItem("Table");
    ui->boxTypeMessage->addItem("Partner");

    setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint  );
    setWindowTitle( QApplication::translate("Chat", "Chat") );

    ui->viewMessage->setModel(m_model);
}

ChatForm::~ChatForm()
{
    delete ui;
}

void ChatForm::addTables(int tables)
{
    // clear previous tables
    qDeleteAll( ui->gbListTable->findChildren<QCheckBox *>(QString(), Qt::FindDirectChildrenOnly));

    unsigned int column {0};
    unsigned int row {0};

    for(auto i=0; i < tables; ++i) {
        QString tableName = "Table " + QString::number(i+1);
        QCheckBox *checkbox = new QCheckBox(tableName.toLatin1());

        m_gridLayout->addWidget(checkbox, row, column);
        column++;

        if(column == 8) {
            row++;
            column = 0;
        }
    }
}

void ChatForm::incomingMessageFromClient(const QString message, const int table, const QString sender)
{
    QString messageClient =  "("+QTime::currentTime().toString() + ") From " +sender+ " (table "+ QString::number(table + 1) +"): "+ message;
    addMessageToListView(messageClient);
}

void ChatForm::on_buttonSend_clicked()
{
    if(ui->lineEditMessage->text().length()>0) {
        prepareMessage();
    }
}

void ChatForm::on_lineEditMessage_returnPressed()
{
    if(ui->lineEditMessage->text().length()>0) {
        prepareMessage();
    }
}

void ChatForm::prepareMessage()
{
    QString message {};

    if(ui->boxTypeMessage->currentIndex() == 0)
    {
        emit sendMessage(ui->lineEditMessage->text());
        message =  "("+QTime::currentTime().toString() + ") To Global: " + ui->lineEditMessage->text();
        addMessageToListView(message);
        ui->lineEditMessage->clear();
    }
    else if (ui->boxTypeMessage->currentIndex() == 1)
    {
        int i {0};
        QVector <int> tables {};

        for(const auto  *checkbox : ui->gbListTable->findChildren<QCheckBox*>())
        {
            if(checkbox->isChecked()) {
                tables.append(i);
            }
            i++;
        }

        if(tables.size() == 0) {
            QMessageBox::critical(this, "Error Send", "No tables selected");
        } else {
            emit sendMessage(ui->lineEditMessage->text(), tables);
            message =  "("+QTime::currentTime().toString() + ") To Tables: " + ui->lineEditMessage->text();
            addMessageToListView(message);
            ui->lineEditMessage->clear();
        }
    }
    else
    {
        int i {0};
        QVector <int> tables {};

        for(const auto  *checkbox : ui->gbListTable->findChildren<QCheckBox*>())
        {
            if(checkbox->isChecked()) {             
                tables.append(i);
            }
            i++;
        }

        if(tables.size() == 0) {
            QMessageBox::critical(this, tr("Error Send"), tr("No tables selected"));
        } else {
            QString player_pair = ui->cbEW->isChecked() ? "EW" : "NS";
            emit sendMessage(ui->lineEditMessage->text(), tables, player_pair);
            message =  "("+QTime::currentTime().toString() + ") To Partner: " + ui->lineEditMessage->text();
            addMessageToListView(message);
            ui->lineEditMessage->clear();
        }
    }
}

void ChatForm::enableBoxPartners(const bool enable)
{
    ui->cbNS->setEnabled(enable);
    ui->cbEW->setEnabled(enable);
}

void ChatForm::addMessageToListView(const QString &message)
{
    m_model->insertRow(m_model->rowCount());
    QModelIndex index = m_model->index(m_model->rowCount()-1);
    m_model->setData(index, message);
}

void ChatForm::on_boxTypeMessage_currentIndexChanged(int index)
{
    if(index == 0) {
        enableBoxPartners(false);
        ui->gbListTable->setEnabled(false);

    } else if (index == 1) {
        enableBoxPartners(false);
        ui->gbListTable->setEnabled(true);

    } else {
        enableBoxPartners(true);
        ui->gbListTable->setEnabled(true);
    }
}
