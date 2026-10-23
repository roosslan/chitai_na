#pragma once

// Логика выбора вопросов, не зависящая от MFC: используется приложением и тестами

#include <cstdint>
#include <random>
#include <vector>

constexpr int RADICAL_COUNT = 214;	/* Количество ключей Канси */
constexpr size_t CHOICE_COUNT = 4;	/* Количество картинок в вопросе */

/* Типы ресурсов ключа */
constexpr int IDB = 0;		/* Картинка */
constexpr int IDW = 1;		/* Написание */
constexpr int IDR = 2;		/* Чтение (пиньинь) */
constexpr int IDE = 3;		/* Значение на английском */
constexpr int IDRU = 4;		/* Значение на русском */
constexpr int IDP = 5;		/* Чтение кириллицей */
constexpr int IDREX = 6;	/* Описание */

/* count уникальных случайных чисел в диапазоне 1..max_value */
std::vector<uint8_t> get_unique_numbers(std::mt19937& gen, size_t count = CHOICE_COUNT, int max_value = RADICAL_COUNT);

/* Случайный элемент из непустого набора */
uint8_t pick_guess_symbol(std::mt19937& gen, const std::vector<uint8_t>& symbols);

/* ID ресурса по имени (например, L"IDW_RADICAL75") для заданного типа; 0, если не найден */
uint16_t str_to_uid(int id_type, const wchar_t* resource_name);
