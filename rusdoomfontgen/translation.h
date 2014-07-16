#pragma once

#include <string>

namespace t {

#ifndef ENGLISH

const std::wstring strtypeface(L"Начертание");
const std::wstring strheight(L"Высота");
const std::wstring strweight(L"Нажим");
const std::wstring stroffset(L"Смещение (кернинг)");
const std::wstring strfiltersselect(L"Фильтры...");
const std::wstring strcreatefont(L"Создать!");
const std::wstring stritalic(L"Италик");
const std::wstring strcodepage(L"Кодовая страница");

const std::wstring strmetrics(L"ACS метрика");



const std::wstring filterswindow(L"Фильтры");
const std::wstring filterswindowaddfilter(L"Добавить");
const std::wstring filterswindowdeletefilter(L"Удалить");
const std::wstring filterswindowupfilter(L"Вверх");
const std::wstring filterswindowdownfilter(L"Вниз");
const std::wstring filtersapplycondition(L"Изменить условие");


const std::wstring borderfilterwnd(L"Фильтр обводки края");
const std::wstring borderfilterdraw(L"Нарисовать");


const std::wstring universalbuttonok(L"ОК");



const std::wstring thresholdfilterwnd(L"Порог");


const std::wstring programname(L"RUSdoomfontgen");

const std::wstring exportend(L"Все изображения экспортированы. Нажмите OK для продолжения.");

#else

const std::wstring strtypeface(L"Typeface");
const std::wstring strheight(L"Height");
const std::wstring strweight(L"Weight");
const std::wstring stroffset(L"Offset (Kerning)");
const std::wstring strfiltersselect(L"Filters...");
const std::wstring strcreatefont(L"Create!");
const std::wstring stritalic(L"Italic");
const std::wstring strcodepage(L"Codepage");

const std::wstring strmetrics(L"ACS metrics");


const std::wstring filterswindow(L"Filters");
const std::wstring filterswindowaddfilter(L"Add filter");
const std::wstring filterswindowdeletefilter(L"Delete");
const std::wstring filterswindowupfilter(L"Up");
const std::wstring filterswindowdownfilter(L"Down");
const std::wstring filtersapplycondition(L"Change condition");

const std::wstring borderfilterwnd(L"Border inking filter");
const std::wstring borderfilterdraw(L"Draw");

const std::wstring thresholdfilterwnd(L"Threshold");

const std::wstring universalbuttonok(L" Ok ");

const std::wstring programname(L"ENGdoomfontgen");

const std::wstring exportend(L"All images was exported. Press OK to continue.");

#endif
}
