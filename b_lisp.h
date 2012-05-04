/*============================================================================*/
/*     ____    _        _    ____    _        _   _     _         __          */
/*    / __ \   \\      //   / __ \   \\      //  | |   / /       /  \         */
/*   / /  \_\   \\    //   / /  \_\   \\    //   | |  / /       / /\ \        */
/*  / /____      \\  //   / /____      \\  //    | | / /       / /  \ \       */
/*  |  ___ \      \\//    |  ___ \      \\//     | |/ /       / /____\ \      */
/*  | /   \ \      //     | /   \ \      //      |   |       / ________ \     */
/*  \ \___/ /     //      \ \___/ /     //       | |\ \     / /        \ \    */
/*   \_____/    _//        \_____/    _//        | | \ \   / /          \ \   */
/*             |_/                   |_/         |_|  \_\ /_/            \_\  */
/*                                                                            */
/*============================================================================*/
#ifndef _B_LISP_H_
#define _B_LISP_H_
/*============================================================================*/
#include <wchar.h>
/*============================================================================*/
/* Некоторые типы                                                             */
/*============================================================================*/
typedef struct bLisp_Script  bLisp_Script;
/*============================================================================*/
/* bLisp_Open                                                                 */
/*                                                                            */
/* описание:                                                                  */
/*     Функция компилирует скрипт и готоваит его к выполнению.                */
/* аргументы:                                                                 */
/*     _code  - указатель на строку с кодом скрипта.                          */
/*     _error - указатель на строку с ошибкой компиляции.                     */
/* возвращает:                                                                */
/*     указатель на созданную структуру скрипта, либо 0 в случае ошибки.      */
/*     если функция вернула 0, то аргумент _error будет указывать на строку   */
/*     с детальным описанием ошибки.                                          */
/* пример:                                                                    */
/*     bLisp_Script *s;                                                       */
/*     bchar *e;                                                              */
/*     s = bLisp_Open(L"(+ 2 3)", &e);                                        */
/*     if(!s)                                                                 */
/*         printf("error here: %s", e);                                       */
/*============================================================================*/
bLisp_Script *bLisp_Open(wchar_t *_code, wchar_t **_error);
/*============================================================================*/
/* bLisp_OpenFile                                                             */
/*                                                                            */
/* описание:                                                                  */
/*     Функция читает заранее скомпилированный код из файла и                 */
/*     готовит его к выполнению.                                              */
/* аргументы:                                                                 */
/*     _filename - имя файла.                                                 */
/* возвращает:                                                                */
/*     указатель на созданную структуру скрипта, либо 0 в случае ошибки.      */
/*============================================================================*/
bLisp_Script *bLisp_OpenFile(wchar_t *_filename);
/*============================================================================*/
/* bLisp_WriteFile                                                            */
/*                                                                            */
/* описание:                                                                  */
/*     Функция записывает скрипт в файл.                                      */
/* аргументы:                                                                 */
/*     _script   - указатель на открытый скрипт.                              */
/*     _filename - имя файла.                                                 */
/* возвращает:                                                                */
/*     btrue в случае успеха или bfalse в случае неудачи.                     */
/*============================================================================*/
int bLisp_WriteFile(bLisp_Script *_script, wchar_t *_filename);
/*============================================================================*/
/* bLisp_Close                                                                */
/*                                                                            */
/* описание:                                                                  */
/*     Функция закрывает ранее открытый скрипт и освобождает всю память       */
/* аргументы:                                                                 */
/*     _script - указатель на ранее открытый скрипт.                          */
/* возвращает:                                                                */
/*     btrue в случае успеха или bfalse в случае неудачи.                     */
/*============================================================================*/
int bLisp_Close(bLisp_Script *_script);
/*============================================================================*/
/* bLisp_Run                                                                  */
/*                                                                            */
/* описание:                                                                  */
/*     Функция запускает скрипт на выполнение.                                */
/* аргументы:                                                                 */
/*     _script - указатель на ранее открытый скрипт.                          */
/*     _result - указатель на строку результата                               */
/* возвращает:                                                                */
/*     0 в случае успеха или строку ошибки в случае неудачи.                  */
/*     В случае успеха _result будет указывать на строку с результатом.       */
/* пример:                                                                    */
/*     bchar *r, *e;                                                          */
/*     e = bLisp_Run(s, &r);                                                  */
/*     if(e)                                                                  */
/*         printf("error here: %s", e);                                       */
/*     else                                                                   */
/*         printf("result is: %s", r);                                        */
/*============================================================================*/
wchar_t *bLisp_Run(bLisp_Script *_script, wchar_t **_result);
/*============================================================================*/
/* bLisp_RegisterFunction                                                     */
/*                                                                            */
/* описание:                                                                  */
/*     Макрос регистрирует функцию для вызова ее в скрипте.                   */
/*     Поддерживаются стандартные типы, кроме float. Указатели на любые типы. */
/*     Все нестандартные типы будут приводится к int.                         */
/* аргументы:                                                                 */
/*     _s  - указатель на открытый скрипт.                                    */
/*     _p  - указатель на функцию.                                            */
/*     _pt - тип указателя на функцию.                                        */
/* возвращает:                                                                */
/*     btrue в случае успеха или bfalse в случае неудачи.                     */
/*     В случае несоответствия казателя на функцию его типу, ваш компилтор    */
/*     выдаст предупреждение.                                                 */
/* пример:                                                                    */
/*     void hello(int x) { printf("%d", x); }                                 */
/*                                                                            */
/*     bLisp_RegisterFunction(s, hello, void (*)(int) );                      */
/*============================================================================*/
#define bLisp_RegisterFunction(_s, _p, _pt) bLisp_RegFunc(_s, L#_p, (_p == (_pt)_p, _p), L#_pt)
/*============================================================================*/
/* bLisp_RegFunc                                                              */
/*                                                                            */
/* описание:                                                                  */
/*     Реализация bLisp_RegisterFunction                                      */
/*============================================================================*/
int bLisp_RegFunc(bLisp_Script *_script, wchar_t *_name, void *_ptr, wchar_t *_arg);
/*============================================================================*/
#endif /* _B_LISP_H_ */