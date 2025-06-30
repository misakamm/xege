#!/bin/bash

# putimage性能对比测试构建和运行脚本
# 用于在WSL环境下构建Windows版本的测试程序

set -e # 遇到错误时退出

cd $(dirname "$0")/.. # 切换到脚本所在目录的上一级（项目根目录）

echo "========================================="
echo "putimage vs putimage_alphablend 性能对比测试"
echo "========================================="

# 检查tasks.sh是否存在
if [ ! -f "tasks.sh" ]; then
    echo "错误: 找不到tasks.sh脚本"
    echo "请确保在项目根目录下运行此脚本"
    exit 1
fi

# 构建项目
echo "步骤 1/3: 构建项目..."
echo "执行: ./tasks.sh --release --load --build"
if ! ./tasks.sh --release --load --build; then
    echo "错误: 项目构建失败"
    exit 1
fi

# 检查测试可执行文件是否存在
TEST_EXE="build/bin/Release/putimage_comparison_test.exe"
if [ ! -f "$TEST_EXE" ]; then
    echo "错误: 找不到测试可执行文件: $TEST_EXE"
    echo "请检查构建是否成功"
    exit 1
fi

echo "步骤 2/3: 找到测试可执行文件: $TEST_EXE"

# 运行测试
echo "步骤 3/3: 运行性能对比测试..."
echo "----------------------------------------"

# 创建结果输出目录
RESULTS_DIR="test_results"
mkdir -p "$RESULTS_DIR"

# 获取当前时间戳
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
RESULT_FILE="$RESULTS_DIR/putimage_comparison_$TIMESTAMP.txt"

echo "测试结果将保存到: $RESULT_FILE"
echo ""

# 运行测试并同时输出到控制台和文件
if "./$TEST_EXE" 2>&1 | tee "$RESULT_FILE"; then
    echo ""
    echo "========================================="
    echo "测试完成!"
    echo "结果已保存到: $RESULT_FILE"
    echo "========================================="

    # 显示简要摘要
    echo ""
    echo "快速摘要 (从结果文件中提取):"
    echo "----------------------------------------"
    if grep -E "(总结|性能差异|Alpha混合比基础版本慢)" "$RESULT_FILE" | head -20; then
        :
    else
        echo "无法提取摘要信息"
    fi

else
    echo "错误: 测试执行失败"
    exit 1
fi

echo ""
echo "如需查看完整结果，请查看文件: $RESULT_FILE"
