# C++ компилятор
CXX = g++

# Флаги компилятора
# -std=c++17: Требуется для std::filesystem и std::string_view
# -Wall -Wextra: Включает большинство полезных предупреждений
# -g: Добавляет отладочную информацию в исполняемый файл
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Флаги линковщика (если понадобятся)
LDFLAGS =

# Директории
SRCDIR = src

# Имя исполняемого файла
TARGET = dendron

# Автоматически находим все .cpp файлы в директории src
SRCS = $(wildcard $(SRCDIR)/*.cpp)

# Генерируем имена объектных файлов (.o) из исходных (.cpp)
OBJS = $(SRCS:.cpp=.o)

# Цель по умолчанию: собирает наш исполняемый файл
.PHONY: all
all: $(TARGET)

# Правило для линковки объектных файлов в исполняемый
$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo "Build finished. Executable is '$(TARGET)'"

# Шаблонное правило для компиляции .cpp файлов в .o
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/tree.h $(SRCDIR)/main.h
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Цель для очистки проекта от сгенерированных файлов
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -f $(TARGET) $(OBJS)

