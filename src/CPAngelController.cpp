#include "cpangel_controller/CPAngelController.h"
#include "ui_CPAngelController.h"
#include <pluginlib/class_list_macros.h>
#include <QMenu>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>

PLUGINLIB_EXPORT_CLASS(control_panel::CPAngelControllerPlugin, control_panel::ControlPanelPlugin)

namespace control_panel
{
CPAngelControllerPlugin::CPAngelControllerPlugin() :
    ui(new Ui::CPAngelControllerPlugin),
    nodelet_priv(new angel_controller::angel_controller_nodelet(false))
{
    ui->setupUi(this);
    ui->label->setText("CP Angel Controller");
    ui->label->setEnabled(false);
}

CPAngelControllerPlugin::~CPAngelControllerPlugin()
{
    delete ui;
}

void CPAngelControllerPlugin::start()
{
    if(!ros::ok())
    {
        ROS_WARN_STREAM("Tried to start a nodelet that wasn't ready!");
        return;
    }
    settings->setValue(uuid.toString() + "/Active", true);
    nodelet_priv->start();
    ui->label->setEnabled(true);
}

void CPAngelControllerPlugin::stop()
{
    ui->label->setEnabled(false);
    nodelet_priv->stop();
    settings->setValue(uuid.toString() + "/Active", false);
}

void CPAngelControllerPlugin::setup()
{
    ui->label->setText(settings->value(uuid.toString() + "/Label", ui->label->text()).toString());
    if(settings->value(uuid.toString() + "/Active", false).toBool())
        start();
}

boost::shared_ptr<nodelet::Nodelet> CPAngelControllerPlugin::getNodelet()
{
    return boost::shared_ptr<nodelet::Nodelet>(nodelet_priv);
}

void CPAngelControllerPlugin::setActive(bool active)
{
    if(active)
        start();
    else
        stop();
}

void CPAngelControllerPlugin::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction("Enable");
    menu.actions()[0]->setCheckable(true);
    menu.actions()[0]->setChecked(nodelet_priv->stat());
    connect(menu.actions()[0], SIGNAL(toggled(bool)), this, SLOT(setActive(bool)));
    menu.addAction("Configure", this, SLOT(configDialog()));
    menu.addAction("Delete", this, SLOT(delete_self()));
    menu.exec(event->globalPos());
}

void CPAngelControllerPlugin::configDialog()
{
    QDialog dialog;
    QGridLayout *layout = new QGridLayout;

    QLabel *labeltxt = new QLabel(tr("Label:"));
    QLineEdit *labeledit = new QLineEdit(ui->label->text());
    layout->addWidget(labeltxt, 0, 0);
    layout->addWidget(labeledit, 0, 1);

    QPushButton *okbutton = new QPushButton(tr("&OK"));
    layout->addWidget(okbutton, 1, 1);

    dialog.setLayout(layout);

    dialog.setWindowTitle("Plugin Configuration - CP Angel Controller");

    connect(okbutton, SIGNAL(clicked()), &dialog, SLOT(accept()));

    if(!dialog.exec())
        return;

    if(ui->label->text() != labeledit->text())
    {
        ui->label->setText(labeledit->text());
        settings->setValue(uuid.toString() + "/Label", labeledit->text());
    }
}

}
