#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#define MAX_FILENAME_LEN 512
#define BUFFER_SIZE 1024

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void connect();
    void transfer();

private slots:
    void on_btnConnect_clicked();

    void on_btnTransfer_clicked();

private:
    Ui::MainWindow *ui;

    int sockfd;
    struct sockaddr_in server_addr;
    int port;
    char filename[MAX_FILENAME_LEN];
    char dir[BUFFER_SIZE];
    int fileSize;
    int getSize;
};

#endif // MAINWINDOW_H
