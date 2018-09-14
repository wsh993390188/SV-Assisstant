#ifndef SV_ASSISTANT_BASIC_WINDOW_H
#define SV_ASSISTANT_BASIC_WINDOW_H

#include <QMainWindow>

namespace Ui {
class SVAssistMainWindow;
}

class SV_ASSISTANT_BASIC_WINDOW : public QMainWindow
{
    Q_OBJECT

public:
    explicit SV_ASSISTANT_BASIC_WINDOW(QWidget *parent = 0);
    ~SV_ASSISTANT_BASIC_WINDOW();
signals:
	void MayUsbDeviceChanged();
private:
    void InitWindows();
	bool nativeEvent(const QByteArray & eventType, void * message, long * result) override;
private:
    Ui::SVAssistMainWindow *ui;
};

#endif // SV_ASSISTANT_BASIC_WINDOW_H
