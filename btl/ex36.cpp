#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Student
{
    int studentID;
    std::string name;
    char Gioi_Tinh;
    int Tuoi;
    double Diem_Toan;
    double Diem_Ly;
    double Diem_Hoa;
    double Diem_Trung_Binh;
    std::string Xep_Loai;

    // Constructor
    Student(int id, const std::string &n, char g, int a, double math, double physics, double chemistry)
        : studentID(id), name(n), Gioi_Tinh(g), Tuoi(a), Diem_Toan(math), Diem_Ly(physics), Diem_Hoa(chemistry)
    {
        Diem_Trung_Binh = (Diem_Toan + Diem_Ly + Diem_Hoa) / 3.0;
        calculateClassification();
    }

    // Phân loại theo GPA
    void calculateClassification()
    {
        if (Diem_Trung_Binh >= 8.0)
        {
            Xep_Loai = "Gioi";
        }
        else if (Diem_Trung_Binh >= 7.0)
        {
            Xep_Loai = "Kha";
        }
        else if (Diem_Trung_Binh >= 5.0)
        {
            Xep_Loai = "Trung binh";
        }
        else
        {
            Xep_Loai = "Yeu";
        }
    }
};

class PupilManager
{
private:
    std::vector<Student> pupils;

public:
    // Đọc danh sách học sinh từ file
    void readFromFile(const std::string &filename)
    {
        std::ifstream inFile(filename);
        if (!inFile.is_open())
        {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        int id, Tuoi;
        std::string name;
        char Gioi_Tinh;
        double math, physics, chemistry;

        while (inFile >> id >> name >> Gioi_Tinh >> Tuoi >> math >> physics >> chemistry)
        {
            pupils.push_back(Student(id, name, Gioi_Tinh, Tuoi, math, physics, chemistry));
        }

        inFile.close();
    }

    // Thêm một học sinh
    void addStudent(const Student &student)
    {
        pupils.push_back(student);
    }

    // Cập nhật thông tin sinh viên theo mã sinh viên
    void updateStudent(int studentID, const Student &updatedStudent)
    {
        auto it = std::find_if(pupils.begin(), pupils.end(),
                               [studentID](const Student &s)
                               { return s.studentID == studentID; });

        if (it != pupils.end())
        {
            *it = updatedStudent;
        }
        else
        {
            std::cerr << "Sinh vien co ID " << studentID << " khong tim thay." << std::endl;
        }
    }

    // Xóa sinh viên theo mã sinh viên
    void removeStudent(int studentID)
    {
        pupils.erase(std::remove_if(pupils.begin(), pupils.end(),
                                    [studentID](const Student &s)
                                    { return s.studentID == studentID; }),
                     pupils.end());
    }

    // Tìm kiếm sinh viên theo tên
    std::vector<Student> searchByName(const std::string &studentName)
    {
        std::vector<Student> result;
        for (const auto &student : pupils)
        {
            if (student.name == studentName)
            {
                result.push_back(student);
            }
        }
        return result;
    }

    // Sắp xếp học sinh theo điểm trung bình (GPA)
    void sortByGPA()
    {
        std::sort(pupils.begin(), pupils.end(), [](const Student &a, const Student &b)
                  { return a.Diem_Trung_Binh > b.Diem_Trung_Binh; });
    }

    // Sắp xếp học sinh theo tên
    void sortByName()
    {
        std::sort(pupils.begin(), pupils.end(), [](const Student &a, const Student &b)
                  { return a.name < b.name; });
    }

    // Hiển thị danh sách sinh viên
    void displayStudents()
    {
        for (const auto &student : pupils)
        {
            std::cout << "ID: " << student.studentID << ", Name: " << student.name
                      << ", GPA: " << student.Diem_Trung_Binh << ", Xep loai: " << student.Xep_Loai << std::endl;
        }
    }

    // Ghi danh sách sinh viên vào hồ sơ
    void writeToFile(const std::string &filename)
    {
        std::ofstream outFile(filename);
        if (!outFile.is_open())
        {
            std::cerr << "Loi mo tap tin : " << filename << std::endl;
            return;
        }

        for (const auto &student : pupils)
        {
            outFile << student.studentID << " " << student.name << " " << student.Gioi_Tinh
                    << " " << student.Tuoi << " " << student.Diem_Toan << " "
                    << student.Diem_Ly << " " << student.Diem_Hoa << std::endl;
        }

        outFile.close();
    }
};

int main()
{
    PupilManager manager;

    // Đọc danh sách học sinh từ file
    manager.readFromFile("pupil.txt");

    // Thêm một học sinh mới
    Student newStudent(1001, "Johns", 'M', 18, 8.5, 7.5, 9.0);
    Student new_Student(1003, "James", 'M', 18, 8.0, 9.5, 9.5);
    manager.addStudent(newStudent);
    manager.addStudent(newStudent);

    // Cập nhật thông tin sinh viên
    Student updatedStudent(1002, "Jane", 'F', 17, 7.8, 8.0, 6.5);
    manager.updateStudent(1002, updatedStudent);

    // Xóa một sinh viên
    manager.removeStudent(1003);

    // Tìm kiếm sinh viên theo tên
    std::vector<Student> searchResult = manager.searchByName("Alice");
    if (!searchResult.empty())
    {
        std::cout << "Sinh vien co ten 'Alice':" << std::endl;
        for (const auto &student : searchResult)
        {
            std::cout << "ID: " << student.studentID << ", Name: " << student.name << std::endl;
        }
    }
    else
    {
        std::cout << "Khong tin thay sinh vien nao co ten 'Alice'." << std::endl;
    }

    // Sắp xếp học sinh theo GPA
    manager.sortByGPA();

    // Hiển thị danh sách sinh viên
    manager.displayStudents();

    // Sắp xếp học sinh theo tên
    manager.sortByName();

    // Ghi danh sách sinh viên vào hồ sơ
    manager.writeToFile("pupil.txt");

    return 0;
}
