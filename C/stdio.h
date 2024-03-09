/**\file*********************************************************************
 *                                                                     \brief
 * Ввод-вывод
 *
 ****************************************************************************
 */

/**\addtogroup C_Library
 *@{*/
/**\addtogroup C_Input_output
 *@{*/

#ifndef __STDC_VERSION_STDIO_H__
#define __STDC_VERSION_STDIO_H__ __STDC_VERSION__
#pragma once

// Универсальнее было бы в stddef.h, но возникает циклическая зависимость
// init.h -> syscall.h -> stddef.h
// Простейшая программа «Привет, Мир!» включает этот заголовок.
#include "../runtime/internal.h"
#include "../runtime/init.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "assert.h"
#include "errno.h"
#include "stdarg.h"
#include "string.h"

// size_t, NULL
#include "stddef.h"


#define _IOFBF 0  /**< Ввод-вывод выполняется через буфер в ОЗУ.     */
#define _IOLBF 1  /**< Ввод-вывод выполняется через буфер построчно. */
#define _IONBF 2  /**< Ввод-вывод выполняется непосредственно.       */

/**
 * Размер буфера для setvbuf().
 */
#define BUFSIZ 8192        // LSB 14.4.64. stdio.h

/**
 * Означает отсутствие данных для чтения.
 */
#define EOF ((int)-1)

/**
 * Гарантированный минимум одновременно открытых файлов.
 *
 * Стандарт требует минимум 8, включая стандартные потоки ввода-вывода (7.23.3/15).
 * LSB 14.4.64 определяет 16.
 */
#ifndef FOPEN_MAX
   #define FOPEN_MAX 16
#endif

#define FILENAME_MAX 4096  // LSB 14.4.64. stdio.h

#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

/**@} C_Input_output */

/**\addtogroup C_runtime_internal
 *@{
 */
typedef struct _IO_FILE {
   int      fd;

   uint8_t  buffering;
   uint8_t  orientation;

   // std::ios_base::iostate  goodbit;   Рабочее состояние.
   uint8_t  bad   :1;   /**< Восстановление невозможно.  */
   uint8_t  eof   :1;   /**< Достигнут конец потока.     */
   uint8_t  fail  :1;   /**< Неудача ввода или вывода.   */

   uint8_t  read        :1;
   uint8_t  write       :1;
   uint8_t  append      :1;
   uint8_t  binary      :1;

};

_Static_assert(sizeof(struct _IO_FILE) == 8);

#define _IO_FILE_UNORDERED     0
#define _IO_FILE_BYTE_ORIENTED 1
#define _IO_FILE_WIDE_ORIENTED 2

_Static_assert(STDIN_FILENO  == 0);
_Static_assert(STDOUT_FILENO == 1);
_Static_assert(STDERR_FILENO == 2);

/**
 * Здесь храним объекты открытых файлов.
 * Казалось бы, следует размещать каждый посредством malloc(), заодно с буфером.
 * Однако:
 *  - буфер нужен не всем;
 *  - потребуется объединять их в список единственно ради fflush(0).
 *
 *  Вопрос с размером и возможностью создания неограниченного количества
 *  остаётся открыт:
 *   - уже сейчас возможно увеличить размер, задав #FOPEN_MAX при трансляции;
 *   - в будущем возможно делать realloc() для таблицы; проблему вероятного
 *     изменения адреса можно решить, возвращая вместо указателя индекс
 *     (заодно закроет доступ к struct _IO_FILE).
 */
_Crt_variable
struct _IO_FILE _IO_FILE_heap[FOPEN_MAX] = {
      [STDIN_FILENO].fd  = STDIN_FILENO,
      [STDOUT_FILENO].fd = STDOUT_FILENO,
      [STDERR_FILENO].fd = STDERR_FILENO,
};

_Crt_variable
struct _IO_FILE * _IO_FILE_last = &_IO_FILE_heap[STDERR_FILENO];

/**@} C_runtime_internal */

/**\addtogroup C_Input_output
 *@{*/

/**
 * Хранит служебную информацию потока ввода-вывода.
 *
 * Копия объекта не обязана служить заменой оригиналу (7.23.3/6).
 */
typedef struct _IO_FILE FILE;

/**
 * Стандартный поток вывода.
 */
_Crt_variable
FILE * stdout = &_IO_FILE_heap[STDOUT_FILENO];
#define stdout stdout

/**
 * Стандартный поток ввода.
 */
_Crt_variable
FILE * stdin = &_IO_FILE_heap[STDIN_FILENO];
#define stdin stdin

/**
 * Стандартный поток вывода сообщений об ошибках.
 */
_Crt_variable
FILE * stderr = &_IO_FILE_heap[STDERR_FILENO];
#define stderr stderr


/**\addtogroup C_file_access 7.23.5 Обращения к файлам
 *@{*/

_Crt_function
int fflush(FILE *stream);

/**
 * 7.23.5.1 Функция fclose.
 *
 * Отправляет изменения буфера потока вывода в файл и закрывает файл.
 *
 * \post    Независимо от успешности сохранения буфера, он и файл теряют связь
 *          с потоком. Автоматически созданные буфера освобождаются.
 * \param   stream   Поток ввода-вывода.
 *
 * \return  0 при успехе, иначе #EOF.
 */
_Crt_function
int fclose(FILE *stream)
{
   assert(stream);

   fflush(stream);
   int r = close(stream->fd);
   stream->bad = 1;
   stream->fd  = 0;
   return r;
}

/**
 * 7.23.5.2 Функция fflush.
 *
 * Отправляет изменения буфера потока вывода в файл.
 *
 * \pre     \a stream открыт для вывода
 *          или для ввода-вывода и последняя операция не была вводом.
 * \post    Индикатор ошибки потока установлен в случае неудачи.
 *
 * \param   stream   Поток ввода-вывода.
 *
 * \return  0 при успехе, иначе #EOF.
 */
_Crt_function
int fflush(FILE *stream)
{
   // TODO
   return 0;
}

/**
 * 7.23.5.3 Функция fopen.
 *
 * Открывает поток ввода-вывода для файла с именем \a filename.
 *
 * \pre     Для режимов "x" файл \a filename должен отсутствовать.
 * \post
 *
 * \param   filename Имя открываемого файла.
 * \param   mode     Режим:
 *          -  "r"   Чтение текста из существующего файла.
 *          -  "w"   Запись текста. При отсутствии файла он будет создан.
 *                            При наличии данных в файле его размер обнуляется.
 *          -  "wx"  Запись текста. Возможна исключительно в создаваемый файл.
 *          -  "a"   Дополнение текста. Выводимые данные добавляются после имеющихся.
 *                            При отсутствии файла он будет создан.
 *          -  "rb"  Чтение произвольных данных.
 *          -  "wb"  Запись произвольных данных в создаваемый или очищенный файл.
 *          -  "wbx" Запись произвольные данных в новый файл.
 *          -  "ab"  Дополнение произвольных данных в файл.
 *                            При отсутствии файла он будет создан.
 *          -  "r+"  Чтение-запись текста из существующего файла.
 *          -  "w+"  Чтение-запись текста из создаваемого или очищенного файла.
 *          -  "w+x" Чтение-запись текста в новый файл.
 *          -  "a+"  Дополнение файла текстом с возможностью изменения имеющегося.
 *          -  "r+b" или "rb+" Соответствуют "r+", но данные произвольны.
 *          -  "w+b" или "wb+" Соответствуют "w+", но данные произвольны.
 *          -  "w+bx" или "wb+x" Соответствуют "w+x", но данные произвольны.
 *          -  "a+b" или "ab+"   Соответствуют "a+", но данные произвольны.
 *          - UB: Для иных вариантов поведение не определено.
 *          - Стандарт позволяет игнорировать лишнее или использовать под свои режимы.
 *
 * \return  Указатель на объект управления потоком.
 *          Нулевой указатель в случае ошибки.
 */
_Crt_function
FILE * fopen(const char * restrict filename, const char * restrict mode)
{
   assert(filename);
   assert(mode);

   FILE * f = NULL;

   // Ищем первый свободный элемент таблицы потоков ввода-вывода.
   // Описатель такого элемента предполагаем равным нулю (значение по умолчанию).
   // Совпадает с STDIN_FILENO, потому пропускаем элементы стандартных потоков.
   // TODO close(STDIN_FILENO); ?
   //
   // При закрытии файла с помощью fclose() происходит освобождение младших
   // элементов. Поскольку открытие-закрытие относительно редкие операции,
   // а таблица небольшого размера, для простоты реализации ищем от младших
   // к старшим (наиболее вероятно, что следующий свободен), а при достижении
   // последнего элемента начинаем поиск сначала и переиспользуем освобождённые.
   //
   // TODO многопоточность.
   FILE * ffree = _IO_FILE_last;
   for (unsigned i = FOPEN_MAX - STDERR_FILENO - 1; i; --i)
   {
      if (++ffree == &_IO_FILE_heap[FOPEN_MAX]) {
         ffree = &_IO_FILE_heap[STDERR_FILENO + 1];
      }
      if (!ffree->fd) {
         // Предварительно описатель недействителен.
         ffree->fd = -1;
         f = ffree;
         _IO_FILE_last = ffree;
         break;
      }
   }
   if (!f) {
      // ENFILE? Подобный случай в GLibc - неудача malloc().
      errno = ENOMEM;
   } else {

      f->read   = 0;
      f->write  = 0;
      f->append = 0;
      f->binary = 0;
      f->buffering   = _IONBF;   // TODO реализовать буферизацию и изменить.
      f->orientation = _IO_FILE_UNORDERED;

      int flags = 0;

      switch (*mode++) {
      default:
      case '\0':
         goto invalid_format;
      case 'r':
         f->read = 1;
         flags |= O_RDONLY;
         break;
      case 'w':
         f->write = 1;
         flags |= O_WRONLY | O_CREAT | O_TRUNC;
         break;
      case 'a':
         f->write  = 1;
         f->append = 1;
         flags |= O_APPEND;
         break;
      }
      //TODO Оставшиеся литеры режима.
      assert(!*mode);

      //TODO Как решить потенциальную гонку, когда путь содержит в себе каталоги?
      //     glibc предоставляет нестандартную fdopen() для возможности
      //     предварительного кордебалета с openat().
      //     Возможно определять наличие каталога в пути и кешировать
      //     описатель директории. Возможно ли ограничиться его сохранением
      //     до последующего вызова fopen()? Или следует распознавать факт подмены?
      f->fd = open(filename, flags, S_IWUSR|S_IRUSR | S_IWGRP|S_IRGRP | S_IWOTH|S_IROTH);
   }

invalid_format:
   if (f->fd == -1) {
      f->fd = 0;
      f = NULL;
   }
   return f;
}

/**@} C_file_access */


/**\addtogroup C_formatted_input_output 7.23.6 Ввод-вывод по формату
 *@{*/

_Crt_function
int vfprintf(FILE * restrict stream, const char * restrict format, va_list arg);

/**
 * 7.23.6.1 Функция fprintf.
 *
 * 2 Выводит в поток \c stream в соответствии со строкой \c format, описывающей
 *   правила преобразования последующих аргументов.
 *
 * 3 Формат является многобайтной последовательностью, начинающейся и
 *   заканчивающийся полным Unicode символом (in initial shift state).
 * Состоит из 0 или более директив:
 * - рядовые символы (отличные от \c %) выводится без изменений;
 * - спецификаторы формата, каждый из которых может привести к преобразованию
 *   и выводу последующих аргументов.
 *
 * \param   format
 * \param   ...
 *
 * \return  Количество выведенных символов, либо отрицательное значение
 *          в случае ошибки вывода или кодирования (в т.ч. если не поддерживается
 *          модификатор размера).
 */
_Crt_function
int fprintf(FILE * restrict stream, const char * restrict format, ...)
{
   assert(stream);
   assert(format);

   va_list args;
   va_start(args, format);
   int r = vfprintf(stream, format, args);
   va_end(args);
   return r;
}


/**
 * 7.23.6.3 Функция printf.
 *
 * Соответствует #fprintf() с аргументом #stdout.
 */
_Crt_function
int printf(const char * restrict format, ...)
{
   assert(format);

   va_list args;
   va_start(args, format);
   int r = vfprintf(stdout, format, args);
   va_end(args);
   return r;
}

_Crt_function
int fputc(int c, FILE *stream);

_Crt_function
int fputs(const char * restrict s, FILE * restrict stream);

/**
 * 7.23.6.8 Функция vfprintf.
 *
 * \param   stream   Поток вывода.
 * \param   format
 TODO Взято из microcaml и требует переработки.

 Спецификация строки формата по ISO/IEC 9899:201x §7.21.6.1 (с сокращениями):
 4. Каждый спецификатор конверсии начинается с символа %, после которого
    следует:
   - Ноль или более флагов (в любой последовательности), модифицирующих
     значение спецификатора.
   - Опциональная минимальная ширина поля. Если значение преобразованного
     аргумента содержит символов менее, чем ширина поля, происходит дополнение
     пробелами (по умолчанию) слева (или справа, в случае наличия флага
     выравнивания по левому краю, описанного далее). Ширина поля имеет форму
     звёздочки * (описанной делее) или натурального числа²⁾.
   - Опциональная точность, определяющая минимальное количество выводимых
     разрядов для преобразований d, i, o, u, x и X, количество знаков после
     десятичного разделителя для преобразований a, A, e, E, f и F,
     максимальное количество значащих цифр для преобразований g и G, либо
     максимальным количеством байт бля вывода для преобразования s³⁾.
     Точность имеет форму точки (.) с последующей звёздочкой * или опциональным
     десятичным целым, при его отсутствии точность принимается равной нулю.
     Если точность сопутствует любому другому спецификатору, поведение не определено.
   - Опциональный модификатор длины, специфицирующий размер аргумента.
   - Символ спецификатора конверсии, определяющий тип преобразования.

 5. Как указано выше, ширина поля и точность могут быть обозначены звёздочкой.
    В таком случае, значение определяется целочисленным аргументом.³⁾ [...]

 6. Символы-флаги и их значения:
   - \c - Результат преобразования выравнивается по левому краю поля.
          (Без данного флага выравнивание по правому краю.) ³⁾
   - \c + Результат преобразования числа со знаком всегда начинается с символов
          плюса или минуса.
   - <tt>' '</tt> (пробел)
         Если первый символ преобразования знакового целого не является \c +
         или \c -, перед числом добавляется пробел. Если указаны оба флага
         "пробел" и \c +, флаг "пробел" игнорируется.
   - \#  Выполняется преобразование в "альтернативную форму". Для конверсии o
         увеличивается точность, в случае необходимости, что бы первым разрядом
         был ноль (если значение и точность равны 0, выводится единственный 0).
         Для конверсии x (и X) ненулевой результат предваряется префиксом 0x
         (соответственно 0X). Для конверсий a, A, e, E, f, F, g и G результат
         преобразования непременно содержит десятичный разделитель, даже если
         за ним не следуют цифры. (Обычно десятичный разделитель выводится в
         случае наличия дробной части.) Для конверсий g и G завершающие нули
         не удаляются из результата. Для остальных конверсий поведение не определено.
   - \c 0  Для конверсий d, i, o, u, x, X, a, A, e, E, f, F, g и G для заполнения
         ширины поля вместо пробелов используются ведущие нули (следующие за
         знаком числа или префиксом системы счисления), кроме случаев
         бесконечности и NaN. В случае наличия одновременно флагов 0 и -, первый
         игнорируется. Для конверсий d, i, o, u, x и X, если указана точность,
         флаг 0 игнорируется. Для остальных конверсий поведение не определено.

 7. Модификаторы длины и их значения (прим.: сокращено, исключены упоминания
   - \c о  спецификаторе конверсии n³⁾; размеры указаны в байтах, а не типах Си):
   - \c hh Указывает, что последующий спецификатор конверсии d, i, o, u, x или X
       применяется к младшему байту аргумента.³⁾
   - \c h  Указывает, что последующий спецификатор конверсии d, i, o, u, x или X
       применяется к двум младшим байтам аргумента.³⁾
   - \c l  Указывает, что последующий спецификатор конверсии d, i, o, u, x или X
       применяется к 64-х разрядному аргументу (long в LP64).
   - \c ll Указывает, что последующий спецификатор конверсии d, i, o, u, x или X
       применяется к 64-х разрядному аргументу (long long в LP64).
   - \c j  Указывает, что последующий спецификатор конверсии d, i, o, u, x или X
       применяется к аргументу intmax_t или uintmax_t.³⁾
   - \c z  Указывает, что последующий спецификатор конверсии d, i, o, u, x или X
       применяется к аргументу size_t.³⁾
   - \c t  Указывает, что последующий спецификатор конверсии d, i, o, u, x или X
       применяется к аргументу ptrdiff_t.³⁾
   - \c L  Указывает, что последующий спецификатор конверсии a, A, e, E, f, F, g
       или G применяется к аргументу long double.³⁾
    Если модификатор длины указан для неупомянутых выше конверсиий, поведение
    не определено.

 8. Спецификаторы конверсий и их значение:
   - \c d,i  Целочисленный аргумент преобразуется в десятичное представление вида
         [-]dddd. Точность определяет минимальное количество выводимых разрядов.
   - \c o,u,x,X  Целочисленный аргумент рассматривается как число без знака и
         выводится в восьмеричном (o), десятичном (u) или шестнадцатеричном
         (x и X) представлении; символы abcdef используются в конверсии x, а
         ABCDEF в конверсии X. Точность определяет минимальное количество
         выводимых разрядов; если значение представимо меньшим количеством цифр,
         в старших разрядах выводятся незначащие нули. Точность по умолчанию 1.
         При преобразовании нуля с нулевой точностью символы не выводятся.
   - \c f,F  Вещественный аргумент выводится в виде десятичной дроби [-]ddd.ddd,
         где количество разрядов после десятичного разделителя равно значению
         точности. Если точность не указана, принимается 6; если точность 0 и
         флаг # не указан, десятичный разделитель не выводится. При наличии
         десятичного разделителя, как минимум один разряд выводится перед ним.
         Значение округляется до соответствующего количества знаков.
         Вещественное значение, представляющее собой бесконечность, преобразуется
         к виду [-]inf или [-]infinity, определяемому имплементацией.
         Вещественное значение, представляющее собой NaN ("не число"),
         преобразуется к виду [-]nan или [-]nan(определённая-имплементацией-
         последовательность-символов). Для спецификатора F выводятся
         соответственно INF, INFINITY или NAN.
   - \c e,E  Вещественный аргумент выводится в виде [-]d.ddde±dd, с единственным
         разрядом (отличным от 0 при ненулевом аргументе) перед десятичным
         разделителем и количеством знаков после него, равным точности; в случае
         отсутствия последней, она принимается 6; если точность равна нулю и
         флаг # не указан, десятичный разделитель не выводится. Значение
         округляется до соответствующего количества знаков. Для спецификатора
         E выводится символ E вместо e, представляющим экспоненту. Экспонента
         всегда содержит минимум две цифры, но не более чем необходимо для
         отображения экспоненты. Для нулевого аргумента экспонента равна нулю.
         Вещественное значение, представляющее собой бесконечность или NaN,
         выводится в соответствии со спецификатором f или F.
   - \c g,G  Вещественный аргумент выводится в зависимости от значения и точности
         аналогично f или e (для G соответственно F или E). Пусть P равно
         точности, если она ненулевая, 6, если она не указана, или 1 в случае
         нулевой. Тогда, если при конверсия в стиле E экспонента равна X:
         - при P > X ≥ -4 преобразование выполняется в стиле f (или F), где
           точность принимается P-(X+1).
         - иначе - в стиле e (или E) c точностью P-1.
         В итоге, если флаг # не указан, из дробной части удаляются незначащие
         нули.
         Вещественное значение, представляющее собой бесконечность или NaN,
         выводится в соответствии со спецификатором f или F.
   - \c a,A  Вещественный аргумент выводится в виде [-]0xh.hhhp±d, с одной
         шестнадцатеричной цифрой (отличной от нуля, если аргумент является
         нормализованным числом, и неспецифицированой в ином случае) до
         разделителя и шестнадцатеричных цифр в количестве, равном точности,
         после; если точность не указана, и FLT_RADIX является степенью 2,
         она принимается достаточной для вывода числа без округления;
         [случай для иного FLT_RADIX опущен]
         если точность равна нулю и флаг # не указан, десятичный разделитель
         не выводится. Символы abcdef и ABCDEF используются для конверсий a и A
         соответственно. Конверсия A выводит число с X и P вместо x и p.
         Экспонента всегда представляется как минимум одной цифрой, но не более,
         чем достаточно для представления степени 2. Если значение аргумента
         нулевое, экспонента равна нулю.
         Вещественное значение, представляющее собой бесконечность или NaN,
         выводится в соответствии со спецификатором f или F.⁴⁾
   - \c с    ³⁾ (символ)
   - \c s    ³⁾ (строка)
   - \c p    ³⁾ (указатель)
   - \c n    ³⁾ (указатель на знаковое целое, куда сохраняется количество
            выведенных символов)
   - \c %    Выводится символ %. Конверсия аргументов не выполняется. Полный
         спецификатор конверсии имеет вид %%.

 9. Если спецификатор конверсии невалиден, результат не определён. Если тип
    аргумента не соответствует спецификатору конверсии, поведение не определено.

 * \param   arg
 *
 * \return  Количество выведенных символов, либо отрицательное значение
 *          в случае ошибки вывода или кодирования (в т.ч. если не поддерживается
 *          модификатор размера).
 */
_Crt_function
int vfprintf(FILE * restrict stream, const char * restrict format, va_list arg)
{
   assert(stream);
   assert(format);

   // Приехал айтишник в тундру на митап с чукчей.
   // Вдруг появляется белый медведь. Чукча спокойно надевает лыжи. Айтишник:
   // - Гы, лол! Чукча, ты не заспринтишь более быстро, чем этот северный олень!
   // - А чукче не надо бежать быстрее медведя.
   //   Чукче надо не мешать оптимизатору удалить лишнее.
   int n = 0;
   char buf[sizeof(uintmax_t) * 3]; // 18446744073709551615 и -9223372036854775808

next: ;
   unsigned len = 0;          // Разрядность аргумента (размер в битах).
   unsigned precision = 0;    // Минимальное количество значащих разрядов.
   unsigned width = 0;  // Минимальная ширина поля выводимого аргумента.
   uintmax_t uval = 0;        // Модуль выводимого целого.
   char sign = '\0';          // Знак числа.
   char pad  = ' ';           // Заполнитель поля.
   while (!(n < 0) && *format) {
      int c = *format++;
      if (c != '%') {
ordinary:
         n = fputc(c, stream) == c ? n + 1 : EOF;
      } else {
specification:
         c = *format++;
         switch (c) {
         case '\0':
         default:
            goto invalid_format;
         // TODO
         // Флаги

         // Может быть как флагом (в начальной позиции),
         // так и значением в числе, определяющем ширину поля.
         case '0':
            if (!width) {
               pad = '0';
               goto specification;
            }
            [[fallthrough]];
         // Ширина поля
                   case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
            width = width * 10 + c - '0';
            goto specification;
         case '*':
            width = va_arg(arg, int32_t);
            goto specification;

         // Точность

         // Модификаторы длины
         case 'h':   // short
            if (*format != 'h')  // char
               len = 16;
            else {
               ++format;
               len = 8;
            }
            goto specification;
         case 'l':   // long и long long одинаковы в LP64
            if (*format == 'l')
               ++format;
            [[fallthrough]];
         case 'j':   // intmax_t или uintmax_t
         case 'z':   // size_t
         case 't':   // ptrdiff_t
            len = 64;
            goto specification;

         // Спецификаторы конверсии
         case 'd':   // Десятичное целое со знаком
         case 'i': ;
            intmax_t val;
            assert(len == 64 || len == 32 || len == 16 || len == 8 || len == 0);
            switch (len) {
            default: val = va_arg(arg, int32_t); break;
            case 64: val = va_arg(arg, int64_t); break;
            case 16: val = va_arg(arg, int16_t); break;
            case 8:  val = va_arg(arg, int8_t);  break;
            }
            uval = val < 0 ? -val : val;
            if (val < 0)
               sign = '-';
            goto integer;

         case 'o': assert(!'o');
         case 'x': assert(!'x');
         case 'X': assert(!'X');

         case 'u':   // Десятичное целое без знака
            assert(len == 64 || len == 32 || len == 16 || len == 8 || len == 0);
            switch (len) {
            default: uval = va_arg(arg, uint32_t); break;
            case 64: uval = va_arg(arg, uint64_t); break;
            case 16: uval = va_arg(arg, uint16_t); break;
            case 8:  uval = va_arg(arg, uint8_t);  break;
            }
integer: ;  unsigned dcnt = 0;
            while (1) {
              buf[dcnt++] = uval % 10 + '0';
              if ((uval /= 10) == 0)
                 break;
            }
            if (sign)
               buf[dcnt++] = sign;
            for (unsigned i = dcnt > width ? dcnt : width; i && !(n < 0); --i) {
               int r = fputc(i > dcnt ? pad : buf[i - 1], stream);
               n = r < 0 ? EOF : n + r;
            }
            goto next;

         case 'c':   // Символ
            for (unsigned i = width ? width : 1; i && !(n < 0); --i) {
               int r = fputc(i != 1 ? pad : va_arg(arg, int), stream);
               n = r < 0 ? EOF : n + r;
            }
            goto next;

         case 's':   // Строка
            if (len) goto invalid_format; // TODO wchar_t при l
            // TODO точность
            int r = fputs(va_arg(arg, const char*), stream);
            n = r < 0 ? EOF : n + r;
            goto next;

         case '%':
            if (len) goto invalid_format;
            goto ordinary;
         }
      }
   }
   return n;
invalid_format:
   return EOF;
}

/**@} C_formatted_input_output */


/**\addtogroup C_сharacter_input_output 7.23.7 Ввод-вывод символов
 *@{*/

/**
 * 7.23.7.1 Функция fgetc.
 *
 * \post    Индикатор ошибки потока установлен в случае ошибки ввода.
 *
 * \param   stream   Поток ввода.
 *
 * \return  Полученный из потока символ; EOF в случае ошибки или окончания потока.
 */
_Crt_function
int fgetc(FILE *stream)
{
   assert(stream);

   int r = EOF;
   if (!stream->eof) {
      unsigned char c;
      int n = read(stream->fd, &c, 1);
      if (n < 0) {
         assert(n == -1);
         stream->fail = 1;
      } else if (n == 0) {
         stream->eof = 1;
      } else {
         assert(n == 1);
         r = c;
      }
   }
   return r;
}

/**
 * 7.23.7.3 Функция fputc.
 *
 * Конвертирует \a c в \c unsigned \c char и выводит в текущую позицию вывода потока
 * \a stream, соответственно сдвигая позицию. Если файл не поддерживает
 * позиционирование или открыт в режиме дописывания, символ добавляется в хвост.
 *
 * \pre           Поток \a stream открыт для вывода.
 *
 * \param c       Выводимый символ.
 * \param stream  Поток вывода.
 *
 * \return        Выведенный символ или #EOF в случае ошибки.
 */
_Crt_function
int fputc(int c, FILE *stream)
{
   assert(stream);

   unsigned char octet = c;
   int r = 0;
   while (!r) {   // TODO подробности о случаях, когда возвращает 0 без ошибки.
      r = write(stream->fd, &octet, 1); // -1 в случае ошибки
   }
   return r == 1 ? c : r;
}

/**
 * 7.23.7.4 Функция fputs.
 *
 * Выводит массив символов \a s в поток \a stream. Завершающий <tt>'\0'</tt> не выводится.
 *
 * \param   s        Выводимая строка.
 * \param   stream   Поток вывода.
 *
 * \return  Неотрицательное значение или #EOF в случае ошибки.
 *
 *          Данная реализация подсчитывает количество выведенных символов,
 *          что необходимо для vfprintf().
 */
_Crt_function
int fputs(const char * restrict s, FILE * restrict stream)
{
   assert(s);
   assert(stream);

   int r = 0;
   while (!(r < 0) && *s) {
      int c = *s++;
      r = fputc(c, stream) == c ? r + 1 : EOF;
   }
   return r;
}


/**
 * 7.23.7.7 Функция putc.
 *
 * Повторяет поведение fputs() и определена макросом.
 */
#define putc fputc

/**
 * 7.23.7.9 Функция puts.
 *
 * Выводит массив символов \a s в поток \a stdout и добавляет символ новой строки.
 * Завершающий <tt>'\0'</tt> не выводится.
 *
 * \return  Неотрицательное значение или #EOF в случае ошибки.
 */
_Crt_function
int puts(const char *s)
{
   assert(s);

   int r = fputs(s, stdout);
   if (!(r < 0))
      r = fputc('\n', stdout);
   return r;
}

/**@} C_formatted_input_output */


/**\addtogroup C_direct_input_output 7.23.7 Ввод-вывод символов
 *@{*/

/**
 * 7.23.8.2 Функция fwrite.
 *
 * Выводит в поток \a stream \a nmemb или меньше элементов размера \a size
 * массива \a ptr. Для каждого объекта \a size раз вызывается #fputc(),
 * выводя массив \c unsigned \c char, представляющий объект.
 *
 * Указатель позиции файла увеличивается соответственно количеству выведенных
 * символов. В случае ошибки вывода значение указателя не определено.
 *
 * Если \a nmemb или \a size равны нулю, состояние потока не изменяется.
 *
 * \pre
 * \post
 * \param   ptr      Массив выводимых объектов.
 * \param   size     Размер каждого объекта.
 * \param   nmemb    Количество объектов.
 * \param   stream   Поток вывода.
 *
 * \return  Количество успешно выведенных символов. Может быть меньше \a nmemb
 *          только в случае ошибки.
 */
_Crt_function
size_t fwrite(const void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream)
{
   assert(ptr);
   assert(stream);

   const unsigned char * s = (const char*)ptr;
   int r = 0;
   while (nmemb--) {
      for (size_t i = size; i; --i) {
         int c = *s++;
         if (fputc(c, stream) != c)
            goto enough;
         ++r;
      }
   }
enough:
   return r;
}

/**@} C_direct_input_output */


#endif//__STDC_VERSION_STDIO_H__

/**@} C_Input_output */
/**@} C_Library */