#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>

//タスクを管理するためのクラスと変数，関数を定義
struct Task {
    std::string description; //締め切りの概要
    int year = 0; //締め切り年
    int month = 0; //月
    int day = 0; //日
    int priority = 0; //優先度
};

//グレゴリウス暦1年1月1日からの経過日数を返す関数[参考文献(1)]
static int getdays(int y, int m, int d) 
{
    // 1・2月 → 前年の13・14月
    if (m <= 2)
    {
        --y;
        m += 12;
    }
    int dy = 365 * (y - 1); // 経過年数×365日
    int c = y / 100;
    int dl = (y >> 2) - c + (c >> 2); // うるう年分
    int dm = (m * 979 - 1033) >> 5; // 1月1日から m 月1日までの日数
    return dy + dl + dm + d - 1;
}

//現在の日時を取得し，グレゴリウス暦1年1月1日からの経過日数を返す関数
int currentday() {
    // 現在の日時を取得
    auto currentTime = std::chrono::system_clock::now();
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    // std::tm 構造体に変換
    std::tm timeStruct;
    localtime_s(&timeStruct, &currentTime_t); // Visual C++ での安全なローカルタイム取得

    // 年、月、日を別々の要素としてベクトルに格納
    std::vector<int> timeVector;
    timeVector.push_back(timeStruct.tm_year + 1900); // tm_yearは1900からの年数を表すので、実際の年を得るために1900を加算
    timeVector.push_back(timeStruct.tm_mon + 1); // tm_monは0から始まる月を表すので、実際の月を得るために1を加算
    timeVector.push_back(timeStruct.tm_mday); // 日はそのまま格納

    return getdays(timeVector[0], timeVector[1], timeVector[2]);
}

//タスクを表示
void displayTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) { //タスクが一つも登録されていないとき
        std::cout << "No tasks found.\n";
        return;
    }

    int deadline = 0; //締め切りまでの日数用変数を定義

    std::cout << "Tasks:\n";
    int a = 1; //タスクに番号を表示するための変数
    for (const Task& task : tasks) {
        deadline = getdays(task.year, task.month, task.day) - currentday(); //締め切り日と現在日時のグレゴリウス暦1年1月1日からの経過日数の差を取って，締め切りまでの日数を出す
        std::cout << a++ << ". " << "Description: " << task.description << ", Deadline: " << task.year << "/" << task.month << "/" << task.day << ". " << deadline << "days left" << ", Priority: " << task.priority << '\n'; //やった・yearじゃなくてdeadlineでok
    }
}

//タスクを追加
void addTask(std::vector<Task>& tasks) {
    Task newTask; //タスク追加用クラス
    std::cout << "Enter task description: ";
    std::cin >> newTask.description; //概要を入力させる

    //以下は締め切り年月日，優先度を入力させているが，int型なのでエラー処理をしている
    std::cout << "Enter deadline\n";
    while (true) {
        std::cout << "Year: ";
        if (std::cin >> newTask.year) { //締め切り年を入力
            break;
        }
        else {
            std::cout << "Invalid input. Please try again.\n";
            std::cin.clear(); // エラーフラグをリセット
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // バッファをクリア
        }
    }

    while (true) {
        std::cout << "Month: ";
        if (std::cin >> newTask.month) { //締め切り月を入力
            break;
        }
        else {
            std::cout << "Invalid input. Please try again.\n";
            std::cin.clear(); // エラーフラグをリセット
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // バッファをクリア
        }
    }

    while (true) {
        std::cout << "Day: ";
        if (std::cin >> newTask.day) { //締め切り日を入力
            break;
        }
        else {
            std::cout << "Invalid input. Please try again.\n";
            std::cin.clear(); // エラーフラグをリセット
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // バッファをクリア
        }
    }

    while (true) {
        std::cout << "Enter task priority: ";
        if (std::cin >> newTask.priority) { //優先度を入力
            break;
        }
        else {
            std::cout << "Invalid input. Please try again.\n";
            std::cin.clear(); // エラーフラグをリセット
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // バッファをクリア
        }
    }
    tasks.push_back(newTask); //タスク管理用ベクトルに格納

    std::cout << "Task added successfully!\n";
    displayTasks(tasks); //追加した結果を表示
}

//タスクを消す
void deleteTask(std::vector<Task>& tasks) {
    if (tasks.empty()) { //タスクが一つも登録されていないとき
        std::cout << "No tasks found.\n";
        return;
    }

    //一覧を表示し，どれを消すか選ばせる
    displayTasks(tasks);
    int k = 0;
    //ここもint型に入力させるからエラー処理をしている
    while (true) {
        std::cout << "Enter a number: ";

        if (std::cin >> k) {
            if (k > tasks.size() || k == 0) { //選択肢外の入力がされた場合のエラー処理
                std::cout << "Invalid input. Please try again.\n";
                continue;
            }

            //正常な処理
            tasks.erase(tasks.begin() + (k - 1));
            displayTasks(tasks);
            break;
        }
        else { //負の整数が入力された場合はここで処理される
            std::cout << "Invalid input. Please try again.\n";
            std::cin.clear(); // エラーフラグをリセット
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // バッファをクリア
        }
    }
}

//タスクを保存
void saveTasks(const std::vector<Task>& tasks, const std::string& filename) {
    std::ofstream outputFile(filename); //ファイルを作成または上書き
    if (!outputFile) { //ファイルが作れなかったとき
        std::cerr << "Failed to open the file for writing.\n";
        return;
    }

    for (const Task& task : tasks) { //レポート図2の形式で保存
        outputFile << task.description << " " << task.year << " " << task.month << " " << task.day << " " << task.priority << '\n'; //やった・year month dayを保存するようにする
    }

    outputFile.close();
    std::cout << "Tasks saved to file.\n";
}

//タスクを読み込む
void loadTasks(std::vector<Task>& tasks, const std::string& filename) {
    std::ifstream inputFile(filename); //ファイルを読み込む
    if (!inputFile) { //ファイルが無いとき
        std::cerr << "Failed to open the file for reading.\n";
        return;
    }

    Task task;
    while (inputFile >> task.description >> task.year >> task.month >> task.day >> task.priority) { //ファイルから順番にクラスに格納
        tasks.push_back(task);
    }

    inputFile.close();
    std::cout << "Tasks loaded from file.\n";
}

//ソートに使用する比較関数
bool compareByPriority(const Task& a, const Task& b) {
    return a.priority > b.priority;
}

int main() {
    std::vector<Task> tasks; //タスク管理用ベクトル
    int choice; //操作番号用変数
    int b = 0; //終了前に保存するかどうか判別するための値

    //保存したタスクを読み取るところ
    const std::string filename = "task.txt";
    loadTasks(tasks, filename);

    do {
        std::cout << "===== Task Management App =====\n";
        std::cout << "1. Add task\n"; //タスクを追加
        std::cout << "2. Display tasks\n"; //タスクを表示
        std::cout << "3. Delete task\n"; //タスクを消す
        std::cout << "4. Save tasks to file\n"; //タスクを保存
        std::cout << "5. Sort by priority\n"; //タスクを並び替え
        std::cout << "6. Exit\n"; //終了
        std::cout << "Enter your choice: ";
        if (std::cin >> choice) { //どの操作を行うか入力させる
            switch (choice) { //choiceに応じて場合分け
            case 1:
                addTask(tasks);
                b = 1;
                break;

            case 2:
                displayTasks(tasks);
                b = 0;
                break;

            case 3:
                deleteTask(tasks);
                b = 1;
                break;

            case 4:
                saveTasks(tasks, filename);
                b = 0;
                break;

            case 5:
                std::sort(tasks.begin(), tasks.end(), compareByPriority);
                displayTasks(tasks);
                b = 1;
                break;

            case 6: //終了前に保存するかどうかを確認する
                switch (b) { //bの値に応じて場合分け
                case 0: //既に保存してあるor保存する必要がないとき
                    std::cout << "Exiting the program. Goodbye!\n";
                    break;

                case 1: //保存されていない操作がある時
                    int c; //保存するかどうかをユーザが決めるための変数
                    std::cout << "Caution! You haven't saved your tasks! ";
                    std::cout << "If you want to save them, you can with No 1.\n";
                    std::cout << "Enter a number (0, 1): ";
                    std::cin >> c;
                    switch (c) { //cの値に応じて場合分け
                    case 0: //保存しない
                        std::cout << "Exiting the program. Goodbye!\n";
                        break;

                    case 1: //保存する
                        saveTasks(tasks, filename);
                        std::cout << "Saved successfully! Exiting the program. Goodbye!\n";
                        break;
                    }
                }
                break;

            default: //何か変なことが起こった時
                std::cout << "Invalid choice. Please try again.\n";
            }

            std::cout << '\n';
            continue;
        }

        else { //1-6以外が入力された場合のエラー処理
            std::cout << "Invalid choice. Please try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        
    } while (choice != 6); //6が入力されるまで繰り返す

    return 0;
}