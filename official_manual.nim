# 只导入 Nimib 主模块（不依赖任何子模块）
import nimib

# 1. 手动创建文档对象（用主模块的基础函数）
var myDoc = NbDoc()

# 2. 直接添加 HTML 内容（绕开复杂元素，用原始 HTML 字符串）
myDoc.content.add("<h1>RPN 计算器使用说明书</h1>")
myDoc.content.add("<h2>1. 安装与编译指南</h2>")
myDoc.content.add("<p>环境要求：Ubuntu 20.04+、GCC 9.0+</p>")
myDoc.content.add("<pre><code>cd ~/rpn_calc\n" &
                  "g++ rpn_calc.cpp -o rpn_calc -std=c++11\n" &
                  "./rpn_calc</code></pre>")

myDoc.content.add("<h2>2. 基本命令</h2>")
myDoc.content.add("<ul>")
myDoc.content.add("<li>表达式计算：如 <code>5 3 +</code></li>")
myDoc.content.add("<li><code>clear</code>：清空栈</li>")
myDoc.content.add("<li><code>history</code>：查看历史</li>")
myDoc.content.add("<li><code>q</code>：退出程序</li>")
myDoc.content.add("</ul>")

myDoc.content.add("<h2>3. 常见错误处理</h2>")
myDoc.content.add("<table border='1' cellpadding='8'>")
myDoc.content.add("<tr><th>错误类型</th><th>解决方案</th></tr>")
myDoc.content.add("<tr><td>除零错误</td><td>确保除数不为 0</td></tr>")
myDoc.content.add("<tr><td>无效输入</td><td>仅输入数字和运算符</td></tr>")
myDoc.content.add("</table>")

# 3. 手动生成 HTML 文件（直接写文件，绕开 Nimib 保存函数）
var f = open("rpn_manual.html", fmWrite)
f.write("<!DOCTYPE html><html><body>")
f.write(join(myDoc.content, "\n"))
f.write("</body></html>")
close(f)

# 4. 提示完成
echo "文档生成成功！文件：~/rpn_calc/rpn_manual.html"

