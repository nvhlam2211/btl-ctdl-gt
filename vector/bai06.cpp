#include <iostream>
#include <fstream>
#include <string>

class TreeNode
{
public:
    std::string key;
    std::string value;
    TreeNode *left;
    TreeNode *right;

    TreeNode(const std::string &k, const std::string &v)
        : key(k), value(v), left(nullptr), right(nullptr) {}
};

class Dictionary
{
private:
    TreeNode *root;

    // Phương thức hỗ trợ cho việc duyệt và ghi file
    void inorderTraversal(TreeNode *node, std::ofstream &outFile)
    {
        if (node)
        {
            inorderTraversal(node->left, outFile);
            outFile << node->key << " " << node->value << std::endl;
            inorderTraversal(node->right, outFile);
        }
    }

public:
    Dictionary() : root(nullptr) {}

    // Phương thức đọc từ điển từ file để xây dựng cây
    void readFromFile(const std::string &filename)
    {
        std::ifstream inFile(filename);
        if (!inFile.is_open())
        {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        std::string key, value;
        while (inFile >> key >> value)
        {
            insert(key, value);
        }

        inFile.close();
    }

    // Phương thức ghi từ điển trong cây vào file
    void writeToFile(const std::string &filename)
    {
        std::ofstream outFile(filename);
        if (!outFile.is_open())
        {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        inorderTraversal(root, outFile);

        outFile.close();
    }

    // Phương thức thêm một từ mới vào từ điển đang lưu trong cây
    void insert(const std::string &key, const std::string &value)
    {
        root = insertRecursive(root, key, value);
    }

    // Phương thức đệ quy để thêm một từ mới vào cây
    TreeNode *insertRecursive(TreeNode *node, const std::string &key, const std::string &value)
    {
        if (node == nullptr)
        {
            return new TreeNode(key, value);
        }

        if (key < node->key)
        {
            node->left = insertRecursive(node->left, key, value);
        }
        else if (key > node->key)
        {
            node->right = insertRecursive(node->right, key, value);
        }
        else
        {
            // Nếu từ đã tồn tại, cập nhật giá trị
            node->value = value;
        }

        return node;
    }

    // Phương thức xóa bỏ một từ của từ điển đang lưu trong cây
    void remove(const std::string &key)
    {
        root = removeRecursive(root, key);
    }

    // Phương thức đệ quy để xóa bỏ một từ khỏi cây
    TreeNode *removeRecursive(TreeNode *node, const std::string &key)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (key < node->key)
        {
            node->left = removeRecursive(node->left, key);
        }
        else if (key > node->key)
        {
            node->right = removeRecursive(node->right, key);
        }
        else
        {
            // Node with only one child or no child
            if (node->left == nullptr)
            {
                TreeNode *temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            {
                TreeNode *temp = node->left;
                delete node;
                return temp;
            }

            // Node with two children, get the inorder successor (smallest
            // in the right subtree)
            TreeNode *temp = minValueNode(node->right);

            // Copy the inorder successor's content to this node
            node->key = temp->key;
            node->value = temp->value;

            // Delete the inorder successor
            node->right = removeRecursive(node->right, temp->key);
        }

        return node;
    }

    // Phương thức để tìm giá trị nhỏ nhất trong cây (được sử dụng cho việc xóa node có 2 con)
    TreeNode *minValueNode(TreeNode *node)
    {
        TreeNode *current = node;
        while (current->left != nullptr)
        {
            current = current->left;
        }
        return current;
    }

    // Phương thức cập nhật lại nghĩa một từ đang lưu trong cây
    void update(const std::string &key, const std::string &newValue)
    {
        TreeNode *node = search(key);
        if (node != nullptr)
        {
            node->value = newValue;
        }
    }

    // Phương thức tìm kiếm một từ trong cây
    TreeNode *search(const std::string &key)
    {
        return searchRecursive(root, key);
    }

    // Phương thức đệ quy để tìm kiếm một từ trong cây
    TreeNode *searchRecursive(TreeNode *node, const std::string &key)
    {
        if (node == nullptr || node->key == key)
        {
            return node;
        }

        if (key < node->key)
        {
            return searchRecursive(node->left, key);
        }
        else
        {
            return searchRecursive(node->right, key);
        }
    }
};

int main()
{
    Dictionary dictionary;

    // Đọc từ điển từ file
    dictionary.readFromFile("dictionary.txt");

    // Thêm, xóa, cập nhật từ
    dictionary.insert("hello", "xin chào");
    dictionary.remove("world");
    dictionary.update("goodbye", "tạm biệt");

    // Tìm kiếm từ
    TreeNode *result = dictionary.search("hello");

    if (result != nullptr)
    {
        std::cout << "Meaning of 'hello': " << result->value << std::endl;
    }
    else
    {
        std::cout << "'hello' not found in the dictionary." << std::endl;
    }
    // Ghi từ điển vào file
    dictionary.writeToFile("updated_dictionary.txt");

    return 0;
}