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
#include <windows.h>


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

    //Miscelaneous subfunctions
    bool checkFileNames();
    int ColNums(QString fitxer);
    int RowNums(QString fitxer);

    //Rest of subfunctions
    void plotall();
    void getData(QStringList fitxer, QVector<QVector<QVector<QVector<double>>>> &InputData);
    void findColRowNums(QStringList fitxer);
    int getSingleData(QString fitxer, int i, QVector<QVector<QVector<QVector<double>>>> &InputData);
    int getOneSingleData(QString fitxer, QVector<QVector<double>> &InputData);
    void rescale_rob();
    void AddConstant(QVector<QVector<double>> &vec, int colPlot, double result); //We pass by reference so that the 4D vector can be modified in the function.
    int ColorFunc(int i, double p);
    void Backup();
    void GetBackup();
    void ResetBackup();
    void GetBackupFunc();
    void SaveAll(QStringList fitxers, QStringList fitxersSavedAt);
    void SaveData(QString fitxer, QVector<QVector<QVector<QVector<double>>>> data);
    void SaveSingleData(QString fitxer, QVector<QVector<double>> data);
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
    void MultConsty1(QVector<QVector<double>> &vec, int col);
    void MultConsty2(QVector<QVector<double>> &vec, int col);
    void MultConsty3(QVector<QVector<double>> &vec, double value, int col);
    int spikinator(QVector<QVector<double>> &vec, int points, int col, double sigmas, int modify);
    void linearfit(QVector<QVector<double>> vec, QVector<double> &result, int points, int firstpoint, int col);
    double expectedy(QVector<QVector<double>> vec, QVector<double> result, int points, int firstpoint);
    void SD(QVector<QVector<double>> vec, QVector<double> &result, int points, int firstpoint, int col);
    void InputAction(int action, QVector<double> &inputs);
    double GenerateRandom(double min, double max);
    void VerticalStack(QVector<QVector<double>> &vec, int q, double VerticalShift);
    void toClipboard(const std::string &s);
    void BallBaseline(QVector<QVector<double>> &vec, int col, double radii);
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
    void createNewStep(QString importname, QString exportname, double desiredStep);
    int estimateRowNums(QString importname, double step); // Estimates the number of rows from the first and last digit of 1st column and step.
    bool checkTempExists(QString importnames); // checks if Temp folder exist.
    void setRows(QString inputname, QString exportname, int desiredRows);
    int cleanDatafile(QString datafile, QString outputdatafile, int caption);   // Deletes lines with errors, respecting or not the caption. Returns number of rows deteled.
    void deleteFirstLines(QString datafile, QString outputdatafile, int lines);
    void deleteLastLines(QString datafile, QString outputdatafile, int lines);
    bool ascendingOrder(QString datafile, QString outputdatafile, int ignoreLines);
    bool checkAscendingOrder(QString datafile); //returns true if order is ascending.
    bool descendingOrder(QString datafile, QString outputdatafile, int ignoreLines);
    void invertOrder(QString datafile, QString outputdatafile);
    void LaplacianSmooth(QVector<QVector<double>> &vec, int colPlot, int times);
    void differentiate(QVector<QVector<double>> &vec, int colPlot);
    void integrateFunc(QVector<QVector<double>> &vec, int colPlot);
    void AddConstantX(QVector<QVector<double>> &vec, double result);
    void ExponentiateX(QVector<QVector<double>> &vec, double result);
    void logarithm(QVector<QVector<double>> &vec, int colPlot, double result);
    void logarithmX(QVector<QVector<double>> &vec, double result);
    void Exponentiate(QVector<QVector<double>> &vec, int colPlot, double result);
    void UnitsEnergy(QVector<QVector<double>> &vec, int previousUnit, int newUnit);
    void setPlotToZero(QVector<QVector<double>> &vec, int colPlot);
    double FindGlobalMinY(QVector<QVector<double>> vec, int colPlot);
    int DeleteExceptCol(QVector<QVector<double>> &vec, int result);
    void startValue(QString datafile, QString outputdatafile, double start);
    void averageAllCols(QVector<QVector<double>> &vec);
    void RTheta(QVector<QVector<double>> &vec, int X, int Y);
    void XY(QVector<QVector<double>> &vec, int R, int T);
    void addFiles(QString outFilename, int col, double average);
    double getFirstX(QString file);
    double getLastX(QString file);
    bool checkBackupExists(QString openedNames);
    int RawLineNumbers(QString fitxer);
    bool checkOperateRef();
    bool checkRefFiles();
    bool checkRefColumn();
    bool checkRefNumber();
    bool checkProperRanges();
    void AddReference(QVector<QVector<double>> &vec, int fileNumber, int colPlot, int refCol);
    bool equalDoubles(double double1, double double2, double step, double tolerance);
    void MultiplyReference(QVector<QVector<double>> &vec, int fileNumber, int colPlot, int refCol);
    void SubtractReference(QVector<QVector<double>> &vec, int fileNumber, int colPlot, int refCol);
    void DivideReference(QVector<QVector<double>> &vec, int fileNumber, int colPlot, int refCol);
    void scaleMargins(int colPlot);
    void findMargins(int file, int col);
    void findGlobalMargins();
    void getMyMargins(double &minx, double &maxx, double &miny, double &maxy);
    void updateMyMargins(double &minx, double &maxx, double &miny, double &maxy);
    void MultConstyAreaCol1(QVector<QVector<double>> &vec);
    void MultConstyMaxCol1(QVector<QVector<double>> &vec);
    bool ActionsPerformed(); // Says if actions are performed or not.
    int spikinator1Column(QVector<QVector<double>> &vec, int points, int col, double sigmas, int modify);
    void BallBaseline1Column(QVector<QVector<double>> &vec, int col, double radii);
    bool DivideColumns1Column(QVector<QVector<double>> &vec,int colPlot,int RefCol);
    QString GenerateCode();
    void AppendStrings(QString &code, QVector<QVector<QString>> Actions);
    void ReadCode(QString code);
    void RunOverActions(QVector<QVector<double>> &TemporalArr, int q, int &TempCol, int &TempColRef);
    void PerformCodeActions();
    void CheckExectuedCode(QString code); // checks that code has been properly executed and sets plot variables.
    void MultiplyConstantX(QVector<QVector<double>> &vec, double result);
    bool getDoubleNum(double &num1, QString message, bool messageCols = false); // to get a double value from the user.
    bool getIntNum(int &num1, int min, int max, QString message);
    bool getIntNums(QVector<int> &colsToDelete, int colMin, int colMax); //returns true if user selected valid cols to be deleted.
    QString showString(QVector<int> toShow); //used to debug.
    void DeleteCols(QVector<QVector<double>> &vec, QVector<int> results);
    int Max_X(QVector<QVector<double>> &vec,int Xcol,int Ycol, double energy); // Maximize the X channel for PR.
    void Copy2DIntVector(QVector<QVector<double>> original, QVector<QVector<double>> &created);
    void Change_XY(QVector<QVector<double>> &vec, int Xcol, int Ycol, double theta);
    void AddDummy(QVector<QVector<double>> &vec); // adds a dummy column.
    QVector<int> QStringIntoQVectorInt (QString input);
    void Power(QVector<QVector<double>> &vec, int colPlot, double result);
    void sinus(QVector<QVector<double>> &vec, int colPlot);
    void cosinus(QVector<QVector<double>> &vec, int colPlot);
    void arccosine(QVector<QVector<double>> &vec, int colPlot);
    void arcsine(QVector<QVector<double>> &vec, int colPlot);
    void absoluteValue(QVector<QVector<double>> &vec, int colPlot);
    void clearAllGlobals();
    void DeleteAll(QStringList files);    
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void openPlotFiles(QStringList tempFilenames);


    QString x_axis, y_axis, rememberExt;
    QString startingDirectory = "C://";
    QVector<QString> prevExt; //remember previous extension.
    QString Tempfolder ="/TempPreview";
    QStringList filenames, inputnames, oldinputnames, importnames, exportnames;
    QStringList newFileNames;
    QString newFileDirectory, Logo; //the ones chosen to save as...
    QStringList referencenames, graphNamesLegend;

    bool ZoomDrag =1;
    int flag1; // (0 => 1 file, 1 => all files plotted)
    int deletedLines;
    QVector<int> replacements;
    double VerticalShift=0;
    double previousTheta =0; // Used for function change X Y, in a similar way to VerticalShift, but for angles.
    int ColumnSize, RowSize, ColumnMinSize;  //Number of columns and rows in datafiles. Minimum number of columns in AllData[][]
    int undo = 5; //Maximum amount of times the program allows to undo actions.

    int plotNumLim = 10; //Max number of visible plots, 0 => all files are plotted. The user will be able to select this number, by defect it is 10. Larger numbers imply more RAM being used.
    int plotNum;        // Number of files opened
    int currFilePlotNum;    // Current number of files plotted.
    bool AllColumns = false; // Used to remember previous status. True => AllColumn mode.
    int columnPlot = 2; //column number to plot by default is 2, if it is 0 it will plot all columns.

    int referenceCol = 2; //column number to plot by default is 2.
    int previouscolPlot; //column number to plot by default is 2.
    int ReferenceColumn = 2; //reference column number is by default 2.
    int plottedSingleFile =0; // It is the number of plotted file when we want to plot a different file than the first one. Default is 0.

    int numActions =0; // remembers the previous number of actions before undo.
    int UndoableActions =0; // number of undoable actions.
    bool log = false;   //Whether y axis is in log scale.
    bool cancell = false; //If cancell is pressed in input options.
    //AllData is only used for plotting porpuses.
    QVector<double> Plot_margins; // [0], [1], [2] and [3] are: min_abs_x, max_abs_x, min_abs_y, max_abs_y
    QVector<int> FileColNums, FileRowNums; // [0]=0 (1) files have equal, error flag = 0, (different, error flag = 1) num of rows/cols, [1] min num of rows/files, [2] max num of rows/files
    QVector<QVector<QVector<QVector<double>>>> AllData; //4D Vector for plotting only. [Historical][files][columns][rows], historical [0] is active plot. [1] is older plot, up to [5].
    QVector<QVector<double>> RememberInv; //Used to store double data that cannot be stored in AllActions since that's integer.
    //We backup all actions so the program can execute sequentially them into the files.
    QVector<QVector<QString>> AllFormatActions; //will save all actions for formating: Import/Export. [w][0] for type of action, action number w. next data [0][1]... for data necessary to perform the action.
    // AllFormatActions[0][0] = 1 corresponds replace of string [][1] by new string [][2] inside each datafile.
    // AllFormatActions[0][0] = 2 corresponds to modify extension to [][1].
    // AllFormatActions[0][0] = 3 corresponds to modify the step of the first column, therefore changing the number of rows for each datafile.
    // AllFormatActions[0][0] = 4 corresponds to set row number, therefore changing the number of rows for each datafile.

    QVector<int> ActionCol; // Action number that is run over a single columnPlot, and that can be run over all columns (if columnPlot =0 or AllColumns = true).
    QVector<QVector<QString>> AllActions; //First indice for a new action, second index [0][0] for number of action, next indexes [0][1]... for data necessary to perform the action.
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
    // AllActions[0][0] = 23 corresponds to Laplacian smoothing.
    // AllActions[0][0] = 24 corresponds to Differentiate
    // AllActions[0][0] = 25 corresponds to Integrate
    // AllActions[0][0] = 26 corresponds to add a constant to X axis.
    // AllActions[0][0] = 27 corresponds to exponentiate X axis.
    // AllActions[0][0] = 28 corresponds to logarithm of a column Y.
    // AllActions[0][0] = 29 corresponds to exponentiate column X.
    // AllActions[0][0] = 30 corresponds to exponentiate a column Y.
    // AllActions[0][0] = 31 corresponds to change of units.
    // AllActions[0][0] = 32 corresponds to calibration.
    // AllActions[0][0] = 33 corresponds to baseline to zero.
    // AllActions[0][0] = 34 corresponds to delete all columns except one.
    // AllActions[0][0] = 35 corresponds to average all columns.
    // AllActions[0][0] = 36 corresponds to calculate R and Theta from X and Y.
    // AllActions[0][0] = 37 corresponds to calculate X and Y from R and Theta.
    // AllActions[0][0] = 38 corresponds to add reference files.
    // AllActions[0][0] = 39 corresponds to multiply reference files.
    // AllActions[0][0] = 40 corresponds to subtract reference files.
    // AllActions[0][0] = 41 corresponds to divide reference files.
    // AllActions[0][0] = 42 corresponds to normalize cols to 1st col by area.
    // AllActions[0][0] = 43 corresponds to normalize cols to 1st col by max.
    // AllActions[0][0] = 44 MultiplyConstantX.
    // AllActions[0][0] = 45 corresponds to delete a few columns.
    // AllActions[0][0] = 46 maximalize X channel.
    // AllActions[0][0] = 47 recalculate X Y channels for a new theta.
    // AllActions[0][0] = 48 Power
    // AllActions[0][0] = 49 Power
    // AllActions[0][0] = 50 Sinus Col Y
    // AllActions[0][0] = 51 Sinus Col X
    // AllActions[0][0] = 52 cosinus
    // AllActions[0][0] = 53 cosinus
    // AllActions[0][0] = 54 arcsine
    // AllActions[0][0] = 55 arccosine
    // AllActions[0][0] = 56 arcsine
    // AllActions[0][0] = 57 arccosine col X


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
   // void on_actionDelete_column_triggered(); //This function has been replaced by Add dummy column.
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
    void on_actionSet_number_triggered();
    void on_actionDelete_lines_without_data_triggered();
    void on_actionDelete_first_lines_triggered();
    void on_actionDelete_last_lines_triggered();
    void on_actionAscending_triggered();
    void on_actionDescending_triggered();
    void on_actionInvert_all_order_triggered();
    void on_actionLaplacian_smoothing_triggered();
    void on_action_Differentiate_triggered();
    void on_actionIntegrate_triggered();
    void on_actionAdd_constant_4_triggered();
    void on_actionExponenciate_triggered();
    void on_actionLogarithm_4_triggered();
    void on_actionLogarithm_3_triggered();
    void on_actionExponenciate_2_triggered();
    void on_actionEnergy_units_triggered();
    void on_actionCallibrate_data_triggered();
    void on_actionSet_spectra_to_zero_triggered();
    void on_actionDelete_all_columns_except_triggered();
    void on_actionSet_start_of_Col_1_triggered();
    void on_actionAverage_all_columns_triggered();
    void on_actionX_Y_R_Theta_triggered();
    void on_actionR_Theta_X_Y_triggered();
    void on_actionAddMultFiles_triggered();
    void on_actionAverageFiles_triggered();
    void on_actionBackup_triggered();
    void on_actionAdd_ref_file_triggered();
    void on_actionSubtract_ref_file_s_triggered();
    void on_actionMultiply_ref_files_s_triggered();
    void on_actionDivide_by_ref_file_s_triggered();
    void on_actionTo_first_column_Area_triggered();
    void on_actionTo_first_column_Max_triggered();
    void on_actionGenerate_code_triggered();
    void on_actionIntroduce_code_triggered();
    void on_actionMultiply_constant_3_triggered();
    void clickedGraph(QMouseEvent *event);
    void selectedGraph();
    void doubleclickedGraph(QMouseEvent *event);
    void on_actionDelete_Columns_triggered();
    void on_actionMaximize_X_at_particular_energy_triggered();
    void on_actionSingle_data_file_triggered();
    void on_actionRecalculate_X_Y_for_given_Theta_triggered();
    void on_actionAdd_Dummy_column_triggered();
    void on_actionPower_triggered();
    void on_actionSinus_triggered();
    void on_actionCosinus_triggered();
    void on_actionPower_2_triggered();
    void on_actionSinus_2_triggered();
    void on_actionCosinus_2_triggered();
    void on_actionArc_sinus_triggered();
    void on_actionArcosine_triggered();
    void on_actionArcsine_triggered();
    void on_actionArccosine_triggered();
    void on_actionAbsolute_value_triggered();
    void on_actionDelete_opened_files_triggered();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
