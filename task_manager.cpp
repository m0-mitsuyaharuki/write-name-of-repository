#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>

//�^�X�N���Ǘ����邽�߂̃N���X�ƕϐ��C�֐����`
struct Task {
    std::string description; //���ߐ؂�̊T�v
    int year = 0; //���ߐ؂�N
    int month = 0; //��
    int day = 0; //��
    int priority = 0; //�D��x
};

//�O���S���E�X��1�N1��1������̌o�ߓ�����Ԃ��֐�[�Q�l����(1)]
static int getdays(int y, int m, int d) 
{
    // 1�E2�� �� �O�N��13�E14��
    if (m <= 2)
    {
        --y;
        m += 12;
    }
    int dy = 365 * (y - 1); // �o�ߔN���~365��
    int c = y / 100;
    int dl = (y >> 2) - c + (c >> 2); // ���邤�N��
    int dm = (m * 979 - 1033) >> 5; // 1��1������ m ��1���܂ł̓���
    return dy + dl + dm + d - 1;
}

//���݂̓������擾���C�O���S���E�X��1�N1��1������̌o�ߓ�����Ԃ��֐�
int currentday() {
    // ���݂̓������擾
    auto currentTime = std::chrono::system_clock::now();
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    // std::tm �\���̂ɕϊ�
    std::tm timeStruct;
    localtime_s(&timeStruct, &currentTime_t); // Visual C++ �ł̈��S�ȃ��[�J���^�C���擾

    // �N�A���A����ʁX�̗v�f�Ƃ��ăx�N�g���Ɋi�[
    std::vector<int> timeVector;
    timeVector.push_back(timeStruct.tm_year + 1900); // tm_year��1900����̔N����\���̂ŁA���ۂ̔N�𓾂邽�߂�1900�����Z
    timeVector.push_back(timeStruct.tm_mon + 1); // tm_mon��0����n�܂錎��\���̂ŁA���ۂ̌��𓾂邽�߂�1�����Z
    timeVector.push_back(timeStruct.tm_mday); // ���͂��̂܂܊i�[

    return getdays(timeVector[0], timeVector[1], timeVector[2]);
}

//�^�X�N��\��
void displayTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) { //�^�X�N������o�^����Ă��Ȃ��Ƃ�
        std::cout << "No tasks found.\n";
        return;
    }

    int deadline = 0; //���ߐ؂�܂ł̓����p�ϐ����`

    std::cout << "Tasks:\n";
    int a = 1; //�^�X�N�ɔԍ���\�����邽�߂̕ϐ�
    for (const Task& task : tasks) {
        deadline = getdays(task.year, task.month, task.day) - currentday(); //���ߐ؂���ƌ��ݓ����̃O���S���E�X��1�N1��1������̌o�ߓ����̍�������āC���ߐ؂�܂ł̓������o��
        std::cout << a++ << ". " << "Description: " << task.description << ", Deadline: " << task.year << "/" << task.month << "/" << task.day << ". " << deadline << "days left" << ", Priority: " << task.priority << '\n'; //������Eyear����Ȃ���deadline��ok
    }
}

//�^�X�N��ǉ�
void addTask(std::vector<Task>& tasks) {
    Task newTask; //�^�X�N�ǉ��p�N���X
    std::cout << "Enter task description: ";
    std::cin >> newTask.description; //�T�v����͂�����

    //�ȉ��͒��ߐ؂�N�����C�D��x����͂����Ă��邪�Cint�^�Ȃ̂ŃG���[���������Ă���
    std::cout << "Enter deadline\n";
    while (true) {
        std::cout << "Year: ";
        if (std::cin >> newTask.year) { //���ߐ؂�N�����
            break;
        }
        else {
            std::cout << "Invalid input. Please try again.\n";
            std::cin.clear(); // �G���[�t���O�����Z�b�g
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // �o�b�t�@���N���A
        }
    }

    while (true) {
        std::cout << "Month: ";
        if (std::cin >> newTask.month) { //���ߐ؂茎�����
            break;
        }
        else {
            std::cout << "Invalid input. Please try again.\n";
            std::cin.clear(); // �G���[�t���O�����Z�b�g
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // �o�b�t�@���N���A
        }
    }

    while (true) {
        std::cout << "Day: ";
        if (std::cin >> newTask.day) { //���ߐ؂�������
            break;
        }
        else {
            std::cout << "Invalid input. Please try again.\n";
            std::cin.clear(); // �G���[�t���O�����Z�b�g
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // �o�b�t�@���N���A
        }
    }

    while (true) {
        std::cout << "Enter task priority: ";
        if (std::cin >> newTask.priority) { //�D��x�����
            break;
        }
        else {
            std::cout << "Invalid input. Please try again.\n";
            std::cin.clear(); // �G���[�t���O�����Z�b�g
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // �o�b�t�@���N���A
        }
    }
    tasks.push_back(newTask); //�^�X�N�Ǘ��p�x�N�g���Ɋi�[

    std::cout << "Task added successfully!\n";
    displayTasks(tasks); //�ǉ��������ʂ�\��
}

//�^�X�N������
void deleteTask(std::vector<Task>& tasks) {
    if (tasks.empty()) { //�^�X�N������o�^����Ă��Ȃ��Ƃ�
        std::cout << "No tasks found.\n";
        return;
    }

    //�ꗗ��\�����C�ǂ���������I�΂���
    displayTasks(tasks);
    int k = 0;
    //������int�^�ɓ��͂����邩��G���[���������Ă���
    while (true) {
        std::cout << "Enter a number: ";

        if (std::cin >> k) {
            if (k > tasks.size() || k == 0) { //�I�����O�̓��͂����ꂽ�ꍇ�̃G���[����
                std::cout << "Invalid input. Please try again.\n";
                continue;
            }

            //����ȏ���
            tasks.erase(tasks.begin() + (k - 1));
            displayTasks(tasks);
            break;
        }
        else { //���̐��������͂��ꂽ�ꍇ�͂����ŏ��������
            std::cout << "Invalid input. Please try again.\n";
            std::cin.clear(); // �G���[�t���O�����Z�b�g
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // �o�b�t�@���N���A
        }
    }
}

//�^�X�N��ۑ�
void saveTasks(const std::vector<Task>& tasks, const std::string& filename) {
    std::ofstream outputFile(filename); //�t�@�C�����쐬�܂��͏㏑��
    if (!outputFile) { //�t�@�C�������Ȃ������Ƃ�
        std::cerr << "Failed to open the file for writing.\n";
        return;
    }

    for (const Task& task : tasks) { //���|�[�g�}2�̌`���ŕۑ�
        outputFile << task.description << " " << task.year << " " << task.month << " " << task.day << " " << task.priority << '\n'; //������Eyear month day��ۑ�����悤�ɂ���
    }

    outputFile.close();
    std::cout << "Tasks saved to file.\n";
}

//�^�X�N��ǂݍ���
void loadTasks(std::vector<Task>& tasks, const std::string& filename) {
    std::ifstream inputFile(filename); //�t�@�C����ǂݍ���
    if (!inputFile) { //�t�@�C���������Ƃ�
        std::cerr << "Failed to open the file for reading.\n";
        return;
    }

    Task task;
    while (inputFile >> task.description >> task.year >> task.month >> task.day >> task.priority) { //�t�@�C�����珇�ԂɃN���X�Ɋi�[
        tasks.push_back(task);
    }

    inputFile.close();
    std::cout << "Tasks loaded from file.\n";
}

//�\�[�g�Ɏg�p�����r�֐�
bool compareByPriority(const Task& a, const Task& b) {
    return a.priority > b.priority;
}

int main() {
    std::vector<Task> tasks; //�^�X�N�Ǘ��p�x�N�g��
    int choice; //����ԍ��p�ϐ�
    int b = 0; //�I���O�ɕۑ����邩�ǂ������ʂ��邽�߂̒l

    //�ۑ������^�X�N��ǂݎ��Ƃ���
    const std::string filename = "task.txt";
    loadTasks(tasks, filename);

    do {
        std::cout << "===== Task Management App =====\n";
        std::cout << "1. Add task\n"; //�^�X�N��ǉ�
        std::cout << "2. Display tasks\n"; //�^�X�N��\��
        std::cout << "3. Delete task\n"; //�^�X�N������
        std::cout << "4. Save tasks to file\n"; //�^�X�N��ۑ�
        std::cout << "5. Sort by priority\n"; //�^�X�N����ёւ�
        std::cout << "6. Exit\n"; //�I��
        std::cout << "Enter your choice: ";
        if (std::cin >> choice) { //�ǂ̑�����s�������͂�����
            switch (choice) { //choice�ɉ����ďꍇ����
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

            case 6: //�I���O�ɕۑ����邩�ǂ������m�F����
                switch (b) { //b�̒l�ɉ����ďꍇ����
                case 0: //���ɕۑ����Ă���or�ۑ�����K�v���Ȃ��Ƃ�
                    std::cout << "Exiting the program. Goodbye!\n";
                    break;

                case 1: //�ۑ�����Ă��Ȃ����삪���鎞
                    int c; //�ۑ����邩�ǂ��������[�U�����߂邽�߂̕ϐ�
                    std::cout << "Caution! You haven't saved your tasks! ";
                    std::cout << "If you want to save them, you can with No 1.\n";
                    std::cout << "Enter a number (0, 1): ";
                    std::cin >> c;
                    switch (c) { //c�̒l�ɉ����ďꍇ����
                    case 0: //�ۑ����Ȃ�
                        std::cout << "Exiting the program. Goodbye!\n";
                        break;

                    case 1: //�ۑ�����
                        saveTasks(tasks, filename);
                        std::cout << "Saved successfully! Exiting the program. Goodbye!\n";
                        break;
                    }
                }
                break;

            default: //�����ςȂ��Ƃ��N��������
                std::cout << "Invalid choice. Please try again.\n";
            }

            std::cout << '\n';
            continue;
        }

        else { //1-6�ȊO�����͂��ꂽ�ꍇ�̃G���[����
            std::cout << "Invalid choice. Please try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        
    } while (choice != 6); //6�����͂����܂ŌJ��Ԃ�

    return 0;
}