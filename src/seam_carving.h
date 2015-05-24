#ifndef SEAM_CARVING_H
#define SEAM_CARVING_H

#include <QtWidgets/QMainWindow>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include "ui_seam_carving.h"
#include <iostream>
#include <string>
#include "image.h"
#include "energy_demo.h"

extern bool demo;

class seam_carving : public QMainWindow
{
	Q_OBJECT

public:
	seam_carving(QWidget *parent = 0);
	~seam_carving();

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
	Ui::seam_carvingClass ui;
	int number_of_seams;
	Image i;
	Mat src;
};

#endif // SEAM_CARVING_H
