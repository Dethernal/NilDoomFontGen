#pragma once

#include <string>

namespace t {

#ifndef ENGLISH

const std::wstring strtypeface(L"����������");
const std::wstring strheight(L"������");
const std::wstring strweight(L"�����");
const std::wstring stroffset(L"�������� (�������)");
const std::wstring strfiltersselect(L"�������...");
const std::wstring strcreatefont(L"�������!");
const std::wstring stritalic(L"������");
const std::wstring strcodepage(L"������� ��������");

const std::wstring strmetrics(L"ACS �������");



const std::wstring filterswindow(L"�������");
const std::wstring filterswindowaddfilter(L"��������");
const std::wstring filterswindowdeletefilter(L"�������");
const std::wstring filterswindowupfilter(L"�����");
const std::wstring filterswindowdownfilter(L"����");
const std::wstring filtersapplycondition(L"�������� �������");


const std::wstring borderfilterwnd(L"������ ������� ����");
const std::wstring borderfilterdraw(L"����������");


const std::wstring universalbuttonok(L"��");



const std::wstring thresholdfilterwnd(L"�����");


const std::wstring programname(L"RUSdoomfontgen");

const std::wstring exportend(L"��� ����������� ��������������. ������� OK ��� �����������.");

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
