#include "seam_carving.h"

seam_carving::seam_carving(QWidget *parent)
	: QMainWindow(parent)
{
	number_of_seams = 0;
	ui.setupUi(this);
	this->move(QPoint(0, 0));
	ui.number->setMaximum(0);
	connect(ui.load, SIGNAL(clicked()), this, SLOT(clickedLoad()));
	connect(ui.addV, SIGNAL(clicked()), this, SLOT(clickedAddV()));
	connect(ui.delV, SIGNAL(clicked()), this, SLOT(clickedDelV()));
	connect(ui.addH, SIGNAL(clicked()), this, SLOT(clickedAddH()));
	connect(ui.delH, SIGNAL(clicked()), this, SLOT(clickedDelH()));
	connect(ui.energy, SIGNAL(clicked()), this, SLOT(clickedEnergy()));
	connect(ui.demo, SIGNAL(clicked()), this, SLOT(clickedDemo()));
	connect(ui.number, SIGNAL(valueChanged(int)), this, SLOT(changedNumberOfSeams(int)));
	connect(ui.sobel, SIGNAL(clicked()), this, SLOT(setSobel()));
	connect(ui.scharr, SIGNAL(clicked()), this, SLOT(setScharr()));
	connect(ui.canny, SIGNAL(clicked()), this, SLOT(setCanny()));
}

void seam_carving::clickedLoad()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open File"), QString(""), QString("All files (*.*);;Picture (*.jpg)"));
	if (path.isEmpty()) return;

	src = imread(path.toStdString());

	if (!src.data)
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Can't open file.");
		messageBox.setFixedSize(500, 200);
		return;
	}

	QFileInfo fileInfo(path);
	QString file_name(fileInfo.fileName());

	i = Image(src, file_name.toStdString());

	if (i.returnHeight() > i.returnWidth())
		ui.number->setMaximum(i.returnHeight());
	else
		ui.number->setMaximum(i.returnWidth());

	namedWindow("Preview", CV_WINDOW_AUTOSIZE);
	i.showImage("Preview");
	cvWaitKey();

}

void seam_carving::clickedAddV()
{
	if (!src.data)
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Can't find file.");
		messageBox.setFixedSize(500, 200);
		return;
	}
	i.findSeams(number_of_seams);
	if (demo) i.addSeams(0);
	i.addSeams(1);
	i.showImage("Preview");
}

void seam_carving::clickedDelV()
{
	if (!src.data)
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Can't find file.");
		messageBox.setFixedSize(500, 200);
		return;
	}
	if (number_of_seams > i.returnWidth()) number_of_seams = i.returnWidth();
	i.deleteVerticalSeams(number_of_seams, "Preview");
	i.showImage("Preview");
}

void seam_carving::clickedAddH()
{
	if (!src.data)
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Can't find file.");
		messageBox.setFixedSize(500, 200);
		return;
	}

	i.rotateImage();
	i.findSeams(number_of_seams);
	if (demo) i.addSeams(0);
	i.addSeams(1);
	i.rotateImage();
	i.showImage("Preview");
}

void seam_carving::clickedDelH()
{
	if (!src.data)
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Can't find file.");
		messageBox.setFixedSize(500, 200);
		return;
	}
	if (number_of_seams > i.returnHeight()) number_of_seams = i.returnHeight();
	i.deleteHorizontalSeams(number_of_seams, "Preview");
	i.showImage("Preview");
}

void seam_carving::clickedEnergy()
{
	if (!src.data)
	{
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Can't find file.");
		messageBox.setFixedSize(500, 200);
		return;
	}

	showEnergyDemo(src);
}

void seam_carving::clickedDemo()
{
	demo = !demo;
}

void seam_carving::changedNumberOfSeams(int k)
{
	number_of_seams = k;
	ui.label_3->setText(QString::number(k));
}

seam_carving::~seam_carving()
{

}
