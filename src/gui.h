#ifndef GUI_H
#define GUI_H

#include <QtWidgets/QMainWindow>
#include <string>
#include "ui_gui.h"
#include "image.h"
#include "energy_demo.h"

class gui : public QMainWindow
{
	Q_OBJECT

public:
	gui(QWidget *parent = 0);
	~gui();
	void errorDialog(std::string s);
	void setMaxNumbersOfSeams();

public slots:
	void clickedLoad();
	void clickedAdd();
	void clickedDel();
	void clickedEnergy();
	void clickedDemo();
	void changedNumberOfVerticalSeams(int k);
	void changedNumberOfHorizontalSeams(int k);
	void setSobel() { edge_type = 0; }
	void setScharr() { edge_type = 1; }
	void setCanny() { edge_type = 2; }

private:
	Ui::guiClass ui;
	int number_of_vertical_seams;
	int number_of_horizontal_seams;
	Image i;
	Mat src;
};

#endif // GUI_H
