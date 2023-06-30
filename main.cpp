#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>

using namespace std;

struct BicycleStrCantAllocateMemory {};

class BicycleStr
{
private:
    char *lineptr = (char*) malloc(sizeof(char));
    unsigned int linesize = 0;

    void realloc_memory(int capasity)
    {
        linesize = capasity;
        lineptr = (char*) realloc(lineptr, sizeof(char) * (capasity + 1));
        if (lineptr) lineptr[capasity] = '\0';
        else throw_cant_allocate_memory();
    };

    void throw_cant_allocate_memory()
    {
        linesize = 0;
        throw BicycleStrCantAllocateMemory();
    };

    void empty_string()
    {
        for (unsigned int i = 0; i < linesize + 1; i++) lineptr[i] = char(0);
    }

public:
    BicycleStr()
    {
        lineptr[0] = '\0';
    };

    BicycleStr (const char& con_char)
    {
        realloc_memory(1);
        if (lineptr) lineptr[0] = con_char;
    }

    BicycleStr (const BicycleStr& con_line)
    {
        realloc_memory(con_line.linesize);

        if (lineptr) for (unsigned int i = 0; i < linesize; i++) lineptr[i] = con_line.lineptr[i];
    };

    BicycleStr (BicycleStr&& other)
    {
        lineptr = other.lineptr;
        linesize = other.linesize;
        other.lineptr = nullptr;
        other.linesize = -1;
    }

    BicycleStr (const char* con_line)
    {
        if (con_line)
        {
            unsigned int fact_size = 0;
            for (int i = 0; i < 5000 && con_line[i] != '\0'; i++)
            {
                fact_size = i;
                if (fact_size > linesize)
                {
                    if (linesize == 0) realloc_memory(1);
                    else if (linesize * 2 > 5000) realloc_memory(5000);
                    else realloc_memory(linesize * 2);
                    if (!bool(lineptr)) return;
                }
                lineptr[i] = con_line[i];
            };
            realloc_memory(fact_size + 1);
        };
    };

    BicycleStr (const string& con_line)
    {
        realloc_memory(con_line.length());

        if (lineptr) for (unsigned int i = 0; i < linesize; i++) lineptr[i] = con_line[i];
    };

    BicycleStr& operator=(const BicycleStr& copy_line)
    {
        realloc_memory(copy_line.linesize);

        if (lineptr) for (unsigned int i = 0; i < linesize; i++) lineptr[i] = copy_line.lineptr[i];

        return *this;
    };

    BicycleStr& operator=(BicycleStr&& other)
    {
        lineptr = other.lineptr;
        linesize = other.linesize;
        other.lineptr = nullptr;
        other.linesize = -1;
        return *this;
    };

    BicycleStr& operator=(const char* con_line)
    {
        if (con_line)
        {
            unsigned int fact_size = 0;
            for (unsigned int i = 0; i < 5000 && con_line[i] != '\0'; i++)
            {
                fact_size = i;
                if (fact_size > linesize)
                {
                    if (linesize == 0) realloc_memory(1);
                    else if (linesize * 2 > 5000) realloc_memory(5000);
                    else realloc_memory(linesize * 2);
                    if (!lineptr) return *this;
                }
                lineptr[i] = con_line[i];
            };
            realloc_memory(fact_size + 1);
        };
        return *this;
    };

    BicycleStr& operator=(const string& copy_line)
    {
        realloc_memory(copy_line.length());

        if (lineptr) for (unsigned int i = 0; i < linesize; i++) lineptr[i] = copy_line[i];

        return *this;
    };

    BicycleStr& operator=(const char& con_char)
    {
        realloc_memory(1);
        if (lineptr) lineptr[0] = con_char;
        return *this;
    }

    BicycleStr& operator+=(const BicycleStr& other)
    {
        if (other.lineptr)
        {
            unsigned int old_linesize = linesize;
            realloc_memory(linesize + other.linesize);

            if (lineptr && old_linesize != linesize) for (unsigned int i = 0; i < other.linesize; i++) lineptr[old_linesize + i] = other.lineptr[i];
        }
        return *this;
    };

    char& operator[] (unsigned int index)
    {
        assert(lineptr);
        assert(index >= 0 && index <= linesize);
        return lineptr[index];
    };

    BicycleStr substr(unsigned int cut_size, unsigned int start = 0)
    {
        BicycleStr subline;
        if (cut_size < 1 || start > length()) return subline;
        if (length() < start+cut_size) cut_size = length() - start;

        subline.realloc_memory(cut_size);

        for (unsigned int i = 0; i < cut_size; i++) subline.lineptr[i] = lineptr[start + i];

        return move(subline);
    };

    int find (const BicycleStr sub_str, unsigned int start = 0)
    {
        if (!bool(sub_str.lineptr) || sub_str.linesize + start > linesize) return -1;

        BicycleStr findline = substr((linesize - start), start);
        BicycleStr subline;
        for (unsigned int i = 0; i + sub_str.linesize <= findline.linesize; i++)
        {
            subline = findline.substr(sub_str.linesize, i);
            if (subline == sub_str) return start + i;
        }
        return -1;
    };

    int count (const BicycleStr sub_str)
    {
        int repeats = 0;
        int pos = this->find(sub_str);

        while (pos != -1)
        {
            repeats++;
            pos = find(sub_str, pos + 1);
        }

        return repeats;
    }

    vector <BicycleStr> split(const BicycleStr delimer, bool allow_empty = true, unsigned int maxsplit = 0)
    {
        vector <BicycleStr> splited_word;
        if (delimer.linesize > 0)
        {
            int pos = this->find(delimer);
            int prev_pos = 0;

            while (pos != -1)
            {
                BicycleStr line = substr(pos - prev_pos, prev_pos);
                prev_pos = pos + delimer.linesize;
                pos = this->find(delimer, prev_pos);
                if (!allow_empty && line.linesize == 0) continue;
                splited_word.push_back(move(line));
                if (maxsplit != 0 && splited_word.size() >= maxsplit) break;
            }
            splited_word.push_back(move(substr(linesize - prev_pos, prev_pos)));
        }
        return move(splited_word);
    }

    unsigned int length()
    {
        return linesize;
    };

    BicycleStr uppercase()
    {
        BicycleStr subcopy;
        subcopy = substr(linesize);
        for (unsigned int i = 0; i < subcopy.linesize; i++)
        {
            cout << int(subcopy.lineptr[i]) << endl;
            if ((subcopy.lineptr[i] > 96 && subcopy.lineptr[i] < 123) || (subcopy.lineptr[i] > -34 && subcopy.lineptr[i] < 0)) subcopy.lineptr[i] -= 32;
        }
        return move(subcopy);
    };

    BicycleStr lowercase()
    {
        BicycleStr subcopy;
        subcopy = substr(linesize);
        for (unsigned int i = 0; i < subcopy.linesize; i++)
        {
            if ((subcopy.lineptr[i] > 64 && subcopy.lineptr[i] < 93) || (subcopy.lineptr[i] > -65 && subcopy.lineptr[i] < -33)) subcopy.lineptr[i] += 32;
        }
        return move(subcopy);
    };

    int to_int()
    {
        int num = 0;
        int first_number_pos = 0;

        for (int i = 0; i < linesize; i++)
        {
            if (substr(1, i).has_numbers())
            {
                first_number_pos = this->find(lineptr[i]);
                break;
            }
        }

        bool is_minus = (first_number_pos > 0 && lineptr[first_number_pos - 1] == '-');

        for (int i = first_number_pos; i < linesize; i++)
        {
            num = num * 10;
            switch(int(lineptr[i]))
            {
            case (48):
                break;
            case (49):
                num += ((-1 * int(is_minus)) + (1 * int(!is_minus)));
                break;
            case (50):
                num += 2 * ((-1 * int(is_minus)) + (1 * int(!is_minus)));
                break;
            case (51):
                num += 3 * ((-1 * int(is_minus)) + (1 * int(!is_minus)));
                break;
            case (52):
                num += 4 * ((-1 * int(is_minus)) + (1 * int(!is_minus)));
                break;
            case (53):
                num += 5 * ((-1 * int(is_minus)) + (1 * int(!is_minus)));
                break;
            case (54):
                num += 6 * ((-1 * int(is_minus)) + (1 * int(!is_minus)));
                break;
            case (55):
                num += 7 * ((-1 * int(is_minus)) + (1 * int(!is_minus)));
                break;
            case (56):
                num += 8 * ((-1 * int(is_minus)) + (1 * int(!is_minus)));
                break;
            case (57):
                num += 9 * ((-1 * int(is_minus)) + (1 * int(!is_minus)));
                break;
            default:
                num = num / 10;
                break;
            }
        }
        return num;
    }

    bool has_numbers()
    {
        for (int i = 0; i < linesize; i++) if (int(lineptr[i]) > 47 && int(lineptr[i]) < 58) return true;
        return false;
    }

    friend ostream& operator<<(ostream& strm, const BicycleStr& strm_bstr);

    friend istream& operator>>(istream& strm, BicycleStr& strm_bstr);

    friend bool operator>(const BicycleStr& first, const BicycleStr& second);

    friend bool operator<(const BicycleStr& first, const BicycleStr& second);

    friend bool operator>=(const BicycleStr& first, const BicycleStr& second);

    friend bool operator<=(const BicycleStr& first, const BicycleStr& second);

    friend bool operator==(const BicycleStr& first, const BicycleStr& second);

    friend bool operator!=(const BicycleStr& first, const BicycleStr& second);

    ~BicycleStr()
    {
        if (lineptr) empty_string();
        free(lineptr);
    };
};

ostream& operator<<(ostream& strm, const BicycleStr& strm_bstr)
{
    if (strm_bstr.lineptr) strm << strm_bstr.lineptr;
    else strm << "";
    return strm;
}

istream& operator>>(istream& strm, BicycleStr& strm_bstr)
{
    string buffer;
    strm >> buffer;
    strm_bstr = buffer;
    return strm;
}

BicycleStr operator+(const BicycleStr& first, const BicycleStr& second)
{
    BicycleStr str_copy = first;
    return str_copy += second;
}

bool operator>(const BicycleStr& first, const BicycleStr& second)
{
    if (!bool(first.lineptr) || !bool(second.lineptr)) return false;

    for (unsigned int i = 0; i < first.linesize && i < second.linesize; i++)
    {
        if (first.lineptr[i] == second.lineptr[i]) continue;
        return first.lineptr[i] > second.lineptr[i];
    }
    return first.linesize > second.linesize;
}

bool operator<(const BicycleStr& first, const BicycleStr& second)
{
    if (!bool(first.lineptr) || !bool(second.lineptr)) return false;

    for (unsigned int i = 0; i < first.linesize && i < second.linesize; i++)
    {
        if (first.lineptr[i] == second.lineptr[i]) continue;
        return first.lineptr[i] < second.lineptr[i];
    }
    return first.linesize < second.linesize;
}

bool operator>=(const BicycleStr& first, const BicycleStr& second)
{
    return !(first < second);
}

bool operator<=(const BicycleStr& first, const BicycleStr& second)
{
    return !(first > second);
}

bool operator==(const BicycleStr& first, const BicycleStr& second)
{
    if (first.linesize != second.linesize) return false;

    for (unsigned int i = 0; i < first.linesize; i++) if (first.lineptr[i] != second.lineptr[i]) return false;

    return true;
}

bool operator!=(const BicycleStr& first, const BicycleStr& second)
{
    return !(first == second);
}

void say_about_help(BicycleStr error_msg = "", BicycleStr command_name = char(8))
{
    cout << "\n" << error_msg << "\nИспользуийте \"-help\" " << command_name << ".\n" << endl;;
    exit(0);
}

void say_about_again_use(BicycleStr about = char(8))
{
    cout << "Ошибка. Параметр \"" << about <<"\" использован повторно.\n" << endl;
    exit(0);
}

int get_command_id(BicycleStr command_name)
{
    if (command_name == "-r") return 1;
    else if (command_name == "-sub") return 2;
    else if (command_name == "-spl") return 3;
    else if (command_name == "-i") return 4;
    else if (command_name == "-help") return 5;
    else if (command_name == "-c") return 6;
}

void show_command_help(BicycleStr command_name)
{
    int command_id = get_command_id(command_name);

    switch (command_id)
    {
    case 1:
        cout << "-r (Reverse):\n" << endl;
        cout << "Последовательный лексикографический порядок." << endl;
        cout << "Т.к. по заданию требуется обратный, этот параметр\nвозвращает изначальный порядок сортировки.\n" << endl;
        break;
    case 2:
        cout << "-sub (Substring):" << endl;
        cout << "-sub <size> [start]\n" << endl;
        cout << "Этот параметр вырезает из каждой изначальной строки\nподстроку указанного размера." << endl;
        cout << "Если границы будущей подстроки выходят за пределы\nизначальной строки, то она будет обрезана, вплоть до пустой (\"\").\n" << endl;
        cout << "Принимает две настройки:\n" << endl;
        cout << "   Обязательная настройка <size> - задаёт размер подстроки.\n" << endl;
        cout << "   Необязательная настройка [start] - позиция в слове,\nс которого начнется вырезание. По-умолчанию равняется 0.\n" << endl;
        break;
    case 3:
        cout << "-spl (Split):" << endl;
        cout << "-spl -e -f -[max_split] <delimer>\n" << endl;
        cout << "Этот параметр разделяет строки на несколько подстрок,\nиспользуя указанный разделитель как ориентир." << endl;
        cout << "Разделитель в подстроки не добавляется.\nНеобязательные параметры должны быть указаны ДО обязательного.\n" << endl;
        cout << "Примает четыре настройки:" << endl;
        cout << "   Обязательная настройка <delimer> - разделитель,\nпо которому будут разделятся строки. Не должен быть пустой (\"\") строкой.\n" << endl;
        cout << "   Необязательная настройка [max_split] - ограничивает\nколичество разделений. По-умолчанию стоит 0, означающий отсутствие ограничений.\n" << endl;
        cout << "   Необязательная настройка -e (empty) - допускает пустые\n(\"\") строки, которые могут образоваться при делении.\n" << endl;
        cout << "   Необязательная настройка -f (fuse) - предварительно\n\"слепляет\" весь список строк в одну. Каждый отдельный\nэлемент списка отделяется пробелом.\n" << endl;
        break;
    case (4):
        cout << "-i (Ignore):\n" << endl;
        cout << "Отключает следующие за этим параметром комманды\n(все, что начинается с \"-\"), воспринимая их как обычные строки.\n" << endl;
        break;
    case (5):
        cout << "\nДопустимые параметры для программы.\nИспользуйте \"-help -<команда>\" для подробностей." << endl;
        cout << "-r\n-c\n-sub\n-spl\n-i\n";
        break;
    case(6):
        cout << "-c (Case):\n" << endl;
        cout << "Включает чувствительность к регистру при сортировке слов.\n" << endl;
        break;
    default:
        say_about_help("Такого параметра нет.");
        break;
    }
}

vector <BicycleStr> split_in_vector(vector <BicycleStr> vec, BicycleStr delimer, bool allow_empty = false, bool fuse = false, int max_splits = 0)
{
    if (delimer == "") say_about_help("Для команды -spl недопустима пустая (\"\") строка как разделитель.", "-spl");

    if (max_splits < 0) max_splits = 0;

    if (fuse)
    {
        BicycleStr fusedline;
        for (int i = 0; i < vec.size(); i++)
        {
            fusedline += vec[i];
            if (i + 1 < vec.size()) fusedline += " ";
        }
        return move(fusedline.split(delimer, allow_empty, max_splits));
    }
    else
    {
        vector <BicycleStr> all_splits;
        for (int i = 0; i < vec.size(); i++)
        {
            vector <BicycleStr> current_splits = vec[i].split(delimer, allow_empty, max_splits);
            for (int b = 0; b < current_splits.size(); b++) all_splits.push_back(move(current_splits[b]));
        }
        return move(all_splits);
    }
}

vector <BicycleStr> substr_in_vector(vector <BicycleStr> vec, int subsize, int start = 0)
{
    if (subsize < 1) say_about_help("Ошибка. Указанный для комманды -sub размер для подстроки недопустим.", "-spl");

    if (start < 0) start = 0;

    for (int i = 0; i < vec.size(); i++) vec[i] = vec[i].substr(subsize, start);

    return move(vec);
}

void show_quoted(BicycleStr text)
{
    cout << "\"" << text << "\"" << endl;
}

void cout_sort (vector <BicycleStr> bic_strs, bool sort_reverse = false, bool case_sensetive = false)
{
    for (int i = 1; i < bic_strs.size(); i++)
    {
        int sorted = i - 1;

        BicycleStr first_str = bic_strs[sorted + 1 * int(sort_reverse)];
        BicycleStr second_str = bic_strs[sorted + 1 * int(!sort_reverse)];

        if (!case_sensetive)
        {
            first_str = first_str.lowercase();
            second_str = second_str.lowercase();
        }

        while (sorted > -1 && first_str > second_str)
        {
            BicycleStr temp = bic_strs[sorted];
            bic_strs[sorted] = move(bic_strs[sorted + 1]);
            bic_strs[sorted + 1] = move(temp);
            sorted--;

            if (sorted < 0) break;

            first_str = bic_strs[sorted + 1 * int(sort_reverse)];
            second_str = bic_strs[sorted + 1 * int(!sort_reverse)];

            if (!case_sensetive)
            {
                first_str = first_str.lowercase();
                second_str = second_str.lowercase();
            }
        }
    }

    for (int i = 0; i < bic_strs.size(); i++) show_quoted(bic_strs[i]);
}

int main(int argc, char* argv[])
{
    setlocale(LC_CTYPE, "Russian");

    if (argc > 1)
    {
        bool r_com = false;

        bool c_com = false;

        bool sub = false;
        bool sub_first = false;
        int sub_size = 0;
        int sub_start = 0;

        bool spl = false;
        bool spl_allow_empty = false;
        bool spl_fuse = false;
        bool spl_max_seted = false;
        int spl_max_split = 0;
        BicycleStr spl_delimer;

        vector <BicycleStr> input;
        for (int i = 1; i < argc; i++) input.push_back(BicycleStr(argv[i]));

        int skipped = 0;

        for (int i = 0; i < 5 && i < input.size(); i++)
        {
            switch (get_command_id(input[i+skipped]))
            {
            case (1):
                if (r_com) say_about_again_use("-r");
                r_com = true;
                break;
            case (2):
                if (sub) say_about_again_use("-sub");
                else if (i + skipped + 1 >= input.size()) say_about_help("Параметру \"-sub\" не указан размер отрезка строки.", "-sub");

                sub = true;
                if (!spl) sub_first = true;

                if (input[i+skipped + 1].has_numbers())
                {
                    sub_size = input[i + skipped + 1].to_int();
                    skipped++;
                }
                else say_about_help("Параметру \"-sub\" не указан размер отрезка строки.", "-sub");

                if (i + skipped + 1 < input.size() && input[i + skipped + 1].has_numbers())
                {
                    sub_start = input[i + skipped + 1].to_int();
                    skipped++;
                }
                break;
            case(3):
                if (spl) say_about_again_use("-spl");
                spl = true;

                for (int b = 1; b < 5 && b + i + skipped < input.size(); b++)
                {
                    if ((input[i + skipped + b][0]) == '-')
                    {
                        if (input[i + skipped + b] == "-e")
                        {
                            if (spl_allow_empty) say_about_again_use("-spl -e");
                            spl_allow_empty = true;
                        }
                        else if (input[i + skipped + b] == "-f")
                        {
                            if (spl_fuse) say_about_again_use("-spl -f");
                            spl_fuse = true;
                        }
                        else if (input[i + skipped + b].has_numbers())
                        {
                            if (spl_max_seted) say_about_again_use("-spl -<max_split>");
                            spl_max_split = input[i + skipped + b].substr(input[i + skipped + b].length(),1).to_int();
                            spl_max_seted = true;
                        }
                        else say_about_help("Нет такого параметра подкоманды -spl, как " + input[i + skipped + b] + ".", "-spl");
                    }
                    else
                    {
                        spl_delimer = input[i + skipped + b];
                        skipped += b;
                        break;
                    }
                }
                break;
            case(4):
                i = input.size() + 1;
                skipped++;
                break;
            case(5):
                if (i + skipped + 1 >= input.size()) show_command_help("-help");
                else show_command_help(input[i + skipped + 1]);
                return 0;
            case (6):
                if (c_com) say_about_again_use("-c");
                c_com = true;
                break;
            default:
                if (i + skipped < input.size() && input[i + skipped][0] == '-') say_about_help("Параметр " + input[i + skipped] + " не распознан.");
                i = input.size() + 1;
                break;
            }
        }

        vector <BicycleStr> cleared_vector;
        for (int i = skipped + int(sub) + int(spl) + int(c_com) + int(r_com); i < input.size(); i++) cleared_vector.push_back(input[i]);

        if (sub && sub_first) cleared_vector = substr_in_vector(cleared_vector, sub_size, sub_start);
        if (spl) cleared_vector = split_in_vector(cleared_vector, spl_delimer, spl_allow_empty, spl_fuse, spl_max_split);
        if (sub && !sub_first) cleared_vector = substr_in_vector(cleared_vector, sub_size, sub_start);

        cout << "\nСлова будут взяты в кавычки после сортировки.\n" << endl;

        cout_sort(cleared_vector, !r_com, c_com);
    }
    else say_about_help("Не было передано ни одного параметра.");
    return 0;
}
