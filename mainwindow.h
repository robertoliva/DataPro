#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QFileDialog>
#include <QGuiApplication>
#include <QIcon>
#include <QMainWindow>
#include <QMessageBox>
#include <QWidget> //to detect keypress
#include <QTextStream>

#include <cmath>
#include <bits/stdc++.h>
#include <regex>
#include <iterator>
#include <complex>
#include <bits/stdc++.h> //to include log
#include <fstream>
#include <fcntl.h>
#include <iostream>
#include <iomanip>
#include <limits>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


typedef std::numeric_limits< double > dbl;

#define PI 3.141592653589793
#define N_E 2.718281828459045

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void plotall(int q);
    void getData(QStringList fitxer, QVector<QVector<QVector<QVector<double>>>> &InputData);
    void findColRowNums(QStringList fitxer);
    int getSingleData(QString fitxer, int i, QVector<QVector<QVector<QVector<double>>>> &InputData);
    int getOneSingleData(QString fitxer, QVector<QVector<double>> &InputData);
    void rescale_rob(int flag);
    void AddConstant(QVector<QVector<double>> &vec, int colPlot, double result); //We pass by reference so that the 4D vector can be modified in the function.
    int ColorFunc(int i, double p);
    int ColNums(QString fitxer);
    int RowNums(QString fitxer);
    void Backup();
    void GetBackup();
    void ResetBackup();
    void GetBackupFunc();
    void SaveAll(QStringList fitxers, QStringList fitxersSavedAt, QVector<QVector<int>> Actions);
    void SaveData(QString fitxer, QVector<QVector<QVector<QVector<double>>>> &data);
    void SaveSingleData(QString fitxer, QVector<QVector<double>> &data);
    int getOneData(QString OneFitxer, QVector<QVector<QVector<QVector<double>>>> &InputData);
    void SubtractColumns(QVector<QVector<double>> &vec,int colPlot,int RefCol);
    void AddColumns(QVector<QVector<double>> &vec,int colPlot,int RefCol);
    void DivideColumns(QVector<QVector<double>> &vec,int colPlot,int RefCol);
    void MultiplyColumns(QVector<QVector<double>> &vec,int colPlot,int RefCol);
    bool validate_double(const std::string& s);
    bool validate_row(const std::string& s);
    bool full_row(const std::string& s); //returns if row contains at least one double    
    void DeleteCol(QVector<QVector<double>> &vec, int result);
    void SwapCols(QVector<QVector<double>> &vec, int result1, int result2);
    void Trim(QVector<QVector<double>> &vec, double result1, double result2);
    void LinearBaseline(QVector<QVector<double>> &vec, int col);
    double Integrate(QVector<QVector<double>> &vec, int col);
    double Maxy(QVector<QVector<double>> &vec, int col);
    void MultConsty(QVector<QVector<double>> &vec, double value, int col);
    int spikinator(QVector<QVector<double>> &vec, int points, int col, double sigmas, int modify);
    void linearfit(QVector<QVector<double>> vec, QVector<double> &result, int points, int firstpoint, int col);
    double expectedy(QVector<QVector<double>> vec, QVector<double> result, int points, int firstpoint);
    void SD(QVector<QVector<double>> vec, QVector<double> &result, int points, int firstpoint, int col);
    void InputAction(int action, QVector<double> &inputs);
    double GenerateRandom(double min, double max);
    void VerticalStack(QVector<QVector<double>> &vec, int q, double VerticalShift);
    void toClipboard(const std::string &s);
    void BallBaseline(QVector<QVector<double>> &vec, int col, double radii);
    double getMyDouble(QString message);
    void Filter(QVector<QVector<double>> &a1, int Col, double lowfreq1, double highfreq1);
    void KKA(QVector<QVector<double>> &vec, int columnPlot);
    double FindMin(QVector<QVector<double>> &vec);
    double FindMax(QVector<QVector<double>> &vec);
    double FindMinFile(QString fitxer);
    double FindMaxFile(QString fitxer);
    void DuplicateCol(QVector<QVector<double>> &vec, int result);
    void findMinCol(QVector<QVector<QVector<double>>> &vec);                                    // Find minimum amount of columns in AllData[][]. Sets ColumnMinSize
    bool DetectForbidden(QString word);                     // Detects forbidden characters for a filename.
    bool DetectSomeForbidden(QString word);                     // Detects forbidden characters for a filename.
    int replaceString(QString nameOfFile, QString nameOfOutputFile, QString find, QString replace, bool writteResult); //We writte the solution if replaceQ is true. Else this function only finds and counts number of characters.
    void SaveAFA(bool rewritte); //Save AllFormatActions. If rewritte is true, it overwrittes it, else it writtes it in /TempPreview/ folder.
    void changeExtension(QString inputname, QString outputname, QString &importnames, QString answer1, QString previousExt, bool rewritte);
    void DeleteTempFolder(QString importnames); //If exists, deletes a temp folder recursivelly.
    void CreateTempFolderFiles(QString importnames);
    double stepNum(QString importname, bool start);
    void createNewStep(QString importname, double desiredStep);

    QString x_axis;
    QString y_axis;
    QString rememberExt;
    QVector<QString> prevExt; //remember previous extension.
    QString Tempfolder ="/TempPreview";
    QStringList filenames, inputnames, importnames, exportnames;
    QStringList newFileNames;
    QString newFileDirectory, Logo; //the ones chosen to save as...
    QStringList referencenames;

    bool ZoomDrag =1;
    int flag1;
    QVector<int> replacements;
    int columnPlot = 2; //column number to plot by default is 2.
    int previouscolPlot; //column number to plot by default is 2.
    int ReferenceColumn = 2; //reference column number is by default 2.
    double VerticalShift=0;
    int ColumnSize, RowSize, ColumnMinSize;  //Number of columns and rows in datafiles. Minimum number of columns in AllData[][]
    int undo = 5; //Maximum amount of times the program allows to undo actions.
    int plotNumLim = 10; //Max number of visible plots, 0 => all files are plotted. The user will be able to select this number, by defect it is 10. Larger numbers imply more RAM being used.
    int plotNum;        //Number of files opened
    bool log = false;   //Whether y axis is in log scale.
    //AllData is only used for plotting porpuses.
    QVector<int> FileColNums, FileRowNums; // [0]=0 (1) files have equal, error flag = 0, (different, error flag = 1) num of rows/cols, [1] min num of rows/files, [2] max num of rows/files
    QVector<QVector<QVector<QVector<double>>>> AllData; //4D Vector for plotting only. [Historical][files][columns][rows], historical [0] is active plot. [1] is older plot, up to [5].
    QVector<QVector<double>> RememberInv; //Used to store double data that cannot be stored in AllActions since that's integer.
    //We backup all actions so the program can execute sequentially them into the files.
    QVector<QVector<QString>> AllFormatActions; //will save all actions for formating: Import/Export. [w][0] for type of action, action number w. next data [0][1]... for data necessary to perform the action.
    // AllFormatActions[0][0] = 1 corresponds replace of string [][1] by new string [][2].
    // AllFormatActions[0][0] = 2 corresponds to modify extension to [][1].

    QVector<QVector<int>> AllActions; //First indice for a new action, second index [0][0] for number of action, next indexes [0][1]... for data necessary to perform the action.
    // AllActions[0][0] = 1 corresponds to new column selected. AllActions[0][1]= column number selected. AllActions[0][2]= previous column value.
    // AllActions[0][0] = 2 corresponds to add constant. AllActions[0][1]= constant number to be added.
    // AllActions[0][0] = 3 corresponds to select column reference. AllActions[0][1]= column number used as reference.
    // AllActions[0][0] = 4 corresponds to subtract column by column reference. AllActions[0][1]= column number, AllActions[0][2] = reference column, AllActions[0][3] = previous column for plot.
    // AllActions[0][0] = 5 corresponds to Divide column by column reference. AllActions[0][1]= column number, AllActions[0][2] = reference column, AllActions[0][3] = previous column for plot
    // AllActions[0][0] = 6 corresponds to Multiply column by column reference. AllActions[0][1]= column number, AllActions[0][2] = reference column, AllActions[0][3] = previous column for plot
    // AllActions[0][0] = 7 corresponds to Delete column. AllActions[0][1]= column number.
    // AllActions[0][0] = 8 corresponds to Swappping columns. AllActions[0][1]= first column to be swapped, AllActions[0][2]= second column to be swapped.
    // AllActions[0][0] = 9 corresponds to Trim. AllActions[0][1]= lowest value, AllActions[0][2]= largest value.
    // AllActions[0][0] = 10 corresponds to LinearBaseline. AllActions[0][1]= column number to perform baseline.
    // AllActions[0][0] = 11 corresponds to NormIntOne. AllActions[0][1]= column number to perform normalization. RememberInv[w][q]= value to multiply for normalization.
    // AllActions[0][0] = 12 corresponds to NormMaxOne. AllActions[0][1]= column number to perform normalization. RememberInv[w][q]= value to multiply for normalization.
    // AllActions[0][0] = 13 corresponds to NormIntFirst. AllActions[0][1]= column number to perform normalization. RememberInv[w][q]= value to multiply for normalization.
    // AllActions[0][0] = 14 corresponds to NormMaxFirst. AllActions[0][1]= column number to perform normalization. RememberInv[w][q]= value to multiply for normalization.
    // AllActions[0][0] = 15 corresponds to MultConsty. AllActions[0][1]= multiplication value. RememberInv[w][q] = column number to perform normalization.
    // AllActions[0][0] = 16 corresponds to Clean_spikes. AllActions[0][1]= selected points. AllActions[0][2]= columnPlot.  AllActions[0][3]= sigmas.
    // AllActions[0][0] = 17 corresponds to Vertical rearrangement. RememberInv[w][0]=VerticalShift.
    // AllActions[0][0] = 18 corresponds to Ball-like baseline. AllActions[0][1]= column number to perform baseline. RememberInv[w][0]=radii.
    // AllActions[0][0] = 19 corresponds to add column by column reference. AllActions[0][1]= column number, AllActions[0][2] = reference column, AllActions[0][3] = previous column for plot.
    // AllActions[0][0] = 20 corresponds to Filter interferences by fast fourier transform analysis. AllActions[0][1]=columnPlot. RememberInv[0][0]= low frequency, RememberInv[0][1]= high frequency.
    // AllActions[0][0] = 21 corresponds to calculate the modulus of the spectra by means of Kramers Kronig relations (see article Scientific Reports | 6:26663 | DOI: 10.1038/srep26663)
    // AllActions[0][0] = 22 corresponds to duplicate columns. AllActions[0][1]= column number.


private slots:

    void on_actionExit_triggered();
    void on_actionOpen_triggered();
    void on_actionRescale_triggered();
    void on_actionNew_triggered();
    void on_actionInformation_triggered();
    void on_actionClean_spikes_triggered();
    void on_actionAll_data_triggered();
    void on_actionOnly_first_datafile_triggered();
    void on_actionAdd_constant_3_triggered();
    void on_actionSet_Plot_Column_triggered();
    void on_actionUndo_triggered();
    void on_actionAdd_reference_column_triggered();
    void on_actionSave_triggered();
    void on_actionSubtract_2_triggered();
    void on_actionDivide_2_triggered();
    void on_actionMultiplu_triggered();
    void on_actionMerge_files_triggered();
    void on_actionAdd_reference_files_triggered();
    void on_actionDelete_column_triggered();
    void on_actionSet_x_column_triggered();
    void on_actionTrim_triggered();
    void on_actionLinear_baseline_triggered();
    void on_actionTo_one_triggered();
    void on_actionTo_one_2_triggered();
    void on_actionTo_first_file_triggered();
    void on_actionTo_first_file_2_triggered();
    void on_actionMultiply_constant_4_triggered();
    void on_actionShow_triggered();
    void on_actionHide_triggered();
    void on_actionActivated_triggered();
    void on_actionDisactivated_triggered();
    void clickedGraph(QMouseEvent *event);
    void doubleclickedGraph(QMouseEvent *event);
    void on_actionBall_like_baseline_triggered();
    void on_actionlogarithmic_triggered();
    void on_actionLinear_triggered();
    void on_actionAdd_reference_triggered();
    void on_actionClean_interferences_by_FFT_triggered();
    void on_actionMerge_files_2_triggered();
    void on_actionSave_as_triggered();
    void on_actionCalculate_modulus_from_KKA_triggered();
    void on_actionSet_max_number_of_plots_triggered();
    void on_actionColumns_to_files_triggered();
    void on_actionDuplicate_Column_triggered();
    void on_actionExport_column_to_single_file_triggered();
    void on_actionSave_in_triggered();
    void on_actionImport_data_triggered();
    void on_actionFiles_triggered();
    void on_actionExtensions_triggered();
    void on_actionReplace_characters_triggered();
    void on_actionExport_data_triggered();
    void on_actionModify_units_2_triggered();
    void on_actionSet_step_triggered();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
