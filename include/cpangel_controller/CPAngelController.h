#ifndef CPANGEL_CONTROLLER_H
#define CPANGEL_CONTROLLER_H

#include <QContextMenuEvent>

#include <control_panel/ControlPanelPlugin.h>
#include <angel_controller/angel_controller_nodelet.hpp>

#include <ros/ros.h>

namespace Ui {
class CPAngelControllerPlugin;
}

namespace control_panel
{
class CPAngelControllerPlugin : public ControlPanelPlugin
{
    Q_OBJECT

public:
    explicit CPAngelControllerPlugin();
    ~CPAngelControllerPlugin();
    void start();
    void stop();
    void setup();
    boost::shared_ptr<nodelet::Nodelet> getNodelet();

public slots:
    void configDialog();
    void setActive(bool active);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    
private:
    Ui::CPAngelControllerPlugin *ui;
    QString name;
    angel_controller::angel_controller_nodelet *nodelet_priv;
};
}

#endif // CPANGEL_CONTROLLER_H
