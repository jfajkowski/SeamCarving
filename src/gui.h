#ifndef GUI_H
#define GUI_H

#include <QtWidgets/QMainWindow>
#include "ui_gui.h"
#include "image.h"
#include "energy_demo.h"

class gui : public QMainWindow
{
	Q_OBJECT

public:
	gui(QWidget *parent = 0);
	~gui();

public slots:
	void clickedLoad();
	void clickedAddV();
	void clickedDelV();
	void clickedAddH();
	void clickedDelH();
	void clickedEnergy();
	void clickedDemo();
	void changedNumberOfSeams(int k);
	void setSobel() { edge_type = 0; }
	void setScharr() { edge_type = 1; }
	void setCanny() { edge_type = 2; }

private:
	Ui::guiClass ui;
	int number_of_seams;
	Image i;
	Mat src;
};

#endif // GUI_H
