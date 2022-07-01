#pragma once
#ifndef _OPTIONS_H_
#define _OPTIONS_H_

static const float CRITICAL_VALUE_RANGE[] = { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.25f, 1.5f, 1.75f, 2.0f };
static float criticalValue = 1.0f;

static const float INTENSITY_RANGE[] = { 10.0f, 50.0f, 100.0f, 200.0f, 400.0f, 800.0f, 1000.0f };
static float intensity = 50.0f;

static const float MAX_SPEED_RANGE[] = { 10.0f, 20.0f, 40.0f, 80.0f, 160.0f, 320.0f };
static float max_speed = 40.0f;

static const int CELLSIZE_RANGE[] = { 5, 10, 20, 40, 80 };
static int cellsize = 10;

static int width = 1002;
static int height = 1002;


static bool drawPoints = false;
static bool drawFillOrLines = true; // true = drawFill, false = drawLines
static bool drawBlobs = false;
static bool paused = false;

#endif