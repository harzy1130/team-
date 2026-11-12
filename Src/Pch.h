#pragma once
//===============================================
//
// プリコンパイル済みヘッダー
//  ここに書いたものは初回のみ解析されるため、コンパイル時間が高速になる。
//  全てのcppからインクルードされる必要がある。
//
//===============================================


//===============================================
//
// 基本
//
//===============================================
#pragma comment(lib,"winmm.lib")

#define NOMINMAX
#include <windows.h>
#include <stdio.h>

#include <wrl/client.h>

//===============================================
//
// STL
//
//===============================================
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <memory>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <filesystem>

//===============================================
//
// Direct3D11
//
//===============================================
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

#include <d3dcommon.h>
#include <dxgi.h>
#include <d3d11_1.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>

// DirectX Tool Kit
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DirectXTKAudioWin8.lib")
#include <SimpleMath.h>
#include <Audio.h>

// DirectX Tex
#pragma comment(lib, "DirectXTex.lib")
#include <DirectXTex.h>

//===============================================
//
// imgui
//
//===============================================
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

//===============================================
// 文字コード変換
//===============================================
#include "strconv.h"

//===============================================
//
// 自作System
//
//===============================================
#include "System/KdSystem.h"
#include <xkeycheck.h>

//valueに入った値をmaxとminをもとに丸める。max>minである必要がある。
float Clamp(float a_value, float a_max, float a_min);
//valueの＋－を1,-1で返す。0の時は＋と判断し1を返す
int GetPlusMinus(float a_value);
//Math::vector2AからBまでの距離を求める
float GetDist(Math::Vector2 a_posA, Math::Vector2 a_posB);
//二つの引数からRECTを作る。a_halfWidth,a_halfHeightは中心からの半幅、半高さ
RECT CreateRect(long a_halfWidth, long a_halfHeight);
//画像のサイズを自動で取得し、RECTを作る。a_pTexはnullptrでないこと
Math::Rectangle CreateRectFromTex(KdTexture* a_pTex, RECT& a_rect = RECT());
//画像のポインタから自動で描画する。単純な描画の時に使う予定。
void DrawTexFromTex(KdTexture* a_pTex, Math::Vector2 a_pos = { 0,0 }, Math::Vector2 a_scale = { 1,1 }, float a_angle = 0.0f, Math::Color a_color = { 1,1,1,1 });
//扇形を描画する。a_posは中心座標、a_radiusは半径、a_angleは角度、a_colorは色、a_degは扇形の角度、a_bCenterは中心から描画するかどうか
void DrawFan(Math::Vector2 a_pos, float a_radius, float a_deg, float a_angle = 90.0f, Math::Color* a_color = &Math::Color{1,1,1,1}, bool a_bCenter = false);
//引数からMath::Matrixを作る。a_posは座標、a_scaleは拡大率、a_angleは角度
Math::Matrix CreateMat(Math::Vector2 a_pos = {0,0}, Math::Vector2 a_scale = { 1,1 }, float a_angle = 0.0f);
//画像の中心から扇状に画像を切り取とって戻り値で返す。a_pTexはnullptrでないこと、a_angleは角度、a_degは扇形の角度
void CreateFanTex(KdTexture* a_pTex,KdTexture* a_pOutTex, Math::Rectangle a_rect, float a_deg, float a_angle = 90.0f, bool a_bCenter = false);
