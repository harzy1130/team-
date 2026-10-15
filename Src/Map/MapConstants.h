#pragma once
//íËêî
const enum MapChip
{
	dirt = 1,
	glass
};
static const int  MapHeight		   = 12;
static const int  MapWidth		   = 32;
const int		  TexSize		   = 64;
const int		  Radius		   = TexSize / 2;
const std::string NEXT_ROW		   = "NEXT_ROW";


const float		  ScreenHeight	   = 720;
const float		  ScreenWidth      = 1280;
const float		  TopEdge		   = ScreenHeight  / 2;
const float		  BottomEdge	   = -ScreenHeight / 2;
const float		  RightEdge		   = ScreenWidth   / 2;
const float		  LeftEdge		   = -ScreenWidth  / 2;