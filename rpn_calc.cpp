#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <stdexcept>

// RPN计算器核心类
class RPNCalculator {
private:
    std::stack<double> numStack;       // 存储数字的栈
    std::vector<std::string> history;  // 计算历史记录（高级功能）

    // 检查栈是否有足够元素
    void checkStackSize(int required, const std::string& op) const {
        if (numStack.size() < required) {
            throw std::runtime_error("错误：栈元素不足，无法执行 " + op + " 操作");
        }
    }

    // 记录计算历史
    void recordHistory(const std::string& expr, double result) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << expr << " = " << result;
        history.push_back(ss.str());
    }

public:
    // 入栈操作
    void push(double value) {
        numStack.push(value);
    }

    // 出栈操作（带检查）
    double pop() {
        checkStackSize(1, "pop");
        double topVal = numStack.top();
        numStack.pop();
        return topVal;
    }

    // 清空栈
    void clearStack() {
        while (!numStack.empty()) {
            numStack.pop();
        }
        std::cout << "栈已清空" << std::endl;
    }

    // 显示当前栈
    void displayStack() const {
        if (numStack.empty()) {
            std::cout << "当前栈为空" << std::endl;
            return;
        }

        std::stack<double> temp = numStack;  // 临时栈用于遍历
        std::cout << "当前栈（从顶到底）：";
        while (!temp.empty()) {
            std::cout << std::fixed << std::setprecision(2) << temp.top() << " ";
            temp.pop();
        }
        std::cout << std::endl;
    }

    // 基础四则运算
    void calculateBasic(const std::string& op) {
        checkStackSize(2, op);
        double b = pop();  // 注意：栈顶是第二个操作数
        double a = pop();  // 栈底是第一个操作数
        double result = 0.0;

        if (op == "+") {
            result = a + b;
        } else if (op == "-") {
            result = a - b;
        } else if (op == "*") {
            result = a * b;
        } else if (op == "/") {
            if (std::fabs(b) < 1e-9) {  // 处理除零错误（浮点数精度）
                throw std::runtime_error("错误：除零操作");
            }
            result = a / b;
        } else {
            throw std::invalid_argument("错误：不支持的基础运算符 " + op);
        }

        push(result);
        std::cout << "计算结果：" << std::fixed << std::setprecision(2) << result << std::endl;
    }

    // 高级数学运算（平方根、幂运算、三角函数）
    void calculateAdvanced(const std::string& op) {
        if (op == "sqrt") {
            checkStackSize(1, op);
            double val = pop();
            if (val < 0) {
                throw std::runtime_error("错误：平方根运算的参数不能为负数");
            }
            double result = std::sqrt(val);
            push(result);
            std::cout << "平方根结果：" << std::fixed << std::setprecision(2) << result << std::endl;
        } else if (op == "pow") {
            checkStackSize(2, op);
            double exp = pop();  // 指数
            double base = pop(); // 底数
            double result = std::pow(base, exp);
            push(result);
            std::cout << "幂运算结果：" << std::fixed << std::setprecision(2) << result << std::endl;
        } else if (op == "sin" || op == "cos" || op == "tan") {
            checkStackSize(1, op);
            double rad = pop();  // 弧度值
            double result = 0.0;
            if (op == "sin") result = std::sin(rad);
            else if (op == "cos") result = std::cos(rad);
            else if (op == "tan") result = std::tan(rad);
            push(result);
            std::cout << op << "运算结果：" << std::fixed << std::setprecision(2) << result << std::endl;
        } else {
            throw std::invalid_argument("错误：不支持的高级运算符 " + op);
        }
    }

    // 斐波那契数列计算（新操作符：fib）
    void calculateFibonacci() {
        checkStackSize(1, "fib");
        int n = static_cast<int>(pop());
        if (n < 0) {
            throw std::runtime_error("错误：斐波那契数列的项数不能为负数");
        }
        if (n > 46) {  // 避免整数溢出（int最大支持第46项）
            throw std::runtime_error("错误：斐波那契项数不能超过46（防止溢出）");
        }

        // 计算斐波那契第n项（F(0)=0, F(1)=1）
        int a = 0, b = 1;
        for (int i = 2; i <= n; ++i) {
            int temp = b;
            b = a + b;
            a = temp;
        }
        double result = (n == 0) ? a : b;
        push(result);
        std::cout << "斐波那契第" << n << "项结果：" << static_cast<int>(result) << std::endl;
    }

    // 批量处理RPN表达式
    double processBatchExpression(const std::string& expr) {
        std::stringstream ss(expr);
        std::string token;
        std::stack<double> tempStack = numStack;  // 保存当前栈状态，批量计算不影响原栈

        while (ss >> token) {
            // 处理数字（整数/浮点数）
            if (isNumber(token)) {
                tempStack.push(std::stod(token));
            }
            // 处理基础运算符
            else if (token == "+" || token == "-" || token == "*" || token == "/") {
                if (tempStack.size() < 2) {
                    throw std::runtime_error("批量计算错误：表达式 " + expr + " 中栈元素不足");
                }
                double b = tempStack.top(); tempStack.pop();
                double a = tempStack.top(); tempStack.pop();
                double res = 0.0;
                if (token == "+") res = a + b;
                else if (token == "-") res = a - b;
                else if (token == "*") res = a * b;
                else if (token == "/") {
                    if (std::fabs(b) < 1e-9) throw std::runtime_error("批量计算错误：除零操作");
                    res = a / b;
                }
                tempStack.push(res);
            }
            // 处理高级运算符（仅支持sqrt）
            else if (token == "sqrt") {
                if (tempStack.empty()) {
                    throw std::runtime_error("批量计算错误：表达式 " + expr + " 中栈为空（sqrt操作）");
                }
                double val = tempStack.top(); tempStack.pop();
                if (val < 0) throw std::runtime_error("批量计算错误：sqrt参数为负数");
                tempStack.push(std::sqrt(val));
            }
            else {
                throw std::invalid_argument("批量计算错误：未知符号 " + token);
            }
        }

        if (tempStack.size() != 1) {
            throw std::runtime_error("批量计算错误：表达式 " + expr + " 语法错误（最终栈元素数不为1）");
        }

        double finalResult = tempStack.top();
        recordHistory(expr, finalResult);  // 记录历史
        return finalResult;
    }

    // 显示计算历史
    void displayHistory() const {
        if (history.empty()) {
            std::cout << "暂无计算历史" << std::endl;
            return;
        }
        std::cout << "计算历史记录：" << std::endl;
        for (size_t i = 0; i < history.size(); ++i) {
            std::cout << i + 1 << ". " << history[i] << std::endl;
        }
    }

    // 辅助函数：判断字符串是否为数字
    static bool isNumber(const std::string& s) {
        std::stringstream ss(s);
        double num;
        // 检查是否能完整解析为浮点数
        return (ss >> num) && (ss.eof());
    }
};

// 命令行交互逻辑
void runCLI() {
    RPNCalculator calc;
    std::string input;
    std::cout << "=== C++ RPN计算器 ===" << std::endl;
    std::cout << "支持功能：" << std::endl;
    std::cout << "  1. 基础运算：+ - * /（例：5 3 +）" << std::endl;
    std::cout << "  2. 高级运算：sqrt（开方）、pow（幂）、sin/cos/tan（三角函数，弧度）" << std::endl;
    std::cout << "  3. 数列运算：fib（斐波那契第n项，例：5 fib → F(5)=5）" << std::endl;
    std::cout << "  4. 栈操作：clear（清空）、show（显示栈）" << std::endl;
    std::cout << "  5. 批量计算：输入完整RPN表达式（例：1 2 + 3 *）" << std::endl;
    std::cout << "  6. 历史记录：history（显示所有计算记录）" << std::endl;
    std::cout << "输入 'q' 退出程序" << std::endl;
    std::cout << "======================" << std::endl;

    while (true) {
        std::cout << "\n> 请输入指令或表达式：";
        std::getline(std::cin, input);
        if (input == "q" || input == "Q") {
            std::cout << "程序退出，感谢使用！" << std::endl;
            break;
        }
        if (input.empty()) continue;

        std::stringstream ss(input);
        std::string token;
        bool isBatchMode = false;  // 是否批量模式（输入完整表达式）

        try {
            // 检查是否为批量表达式（包含运算符）
            std::string tempInput = input;
            if (tempInput.find("+") != std::string::npos || tempInput.find("-") != std::string::npos ||
                tempInput.find("*") != std::string::npos || tempInput.find("/") != std::string::npos ||
                tempInput.find("sqrt") != std::string::npos) {
                isBatchMode = true;
                double batchResult = calc.processBatchExpression(input);
                std::cout << "批量计算结果：" << std::fixed << std::setprecision(2) << batchResult << std::endl;
                continue;
            }

            // 单指令模式
            while (ss >> token) {
                if (RPNCalculator::isNumber(token)) {
                    // 数字入栈
                    calc.push(std::stod(token));
                    std::cout << "已入栈：" << std::fixed << std::setprecision(2) << std::stod(token) << std::endl;
                }
                // 基础运算符
                else if (token == "+" || token == "-" || token == "*" || token == "/") {
                    calc.calculateBasic(token);
                }
                // 高级运算符
                else if (token == "sqrt" || token == "pow" || token == "sin" || token == "cos" || token == "tan") {
                    calc.calculateAdvanced(token);
                }
                // 斐波那契运算
                else if (token == "fib") {
                    calc.calculateFibonacci();
                }
                // 栈操作
                else if (token == "clear") {
                    calc.clearStack();
                }
                else if (token == "show") {
                    calc.displayStack();
                }
                // 历史记录
                else if (token == "history") {
                    calc.displayHistory();
                }
                else {
                    throw std::invalid_argument("未知指令：" + token + "（输入 'q' 退出，或查看帮助）");
                }
            }
        }
        catch (const std::exception& e) {
            // 统一错误处理
            std::cerr << "\033[31m" << e.what() << "\033[0m" << std::endl;
        }
    }
}

int main() {
    runCLI();
    return 0;
}

