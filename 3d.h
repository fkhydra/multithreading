#pragma once
#include <d2d1.h>
#include <math.h>
#include <stdio.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 500

ID2D1Factory* pD2DFactory = NULL;
ID2D1HwndRenderTarget* pRT = NULL;

struct VEKTOR {
 float x;
 float y;
 float z;
};

#define MAX_OBJ_NUM 200000
int viewpoint = -1100;
float rot_degree_x;
float rot_degree_y;
float rot_degree_z;
float rot_degree_x2 = 0;
float rot_degree_y2 = 90.0f;
float rot_degree_z2 = 0;
float Math_PI = 3.14159265358979323846;
float raw_verticesX[MAX_OBJ_NUM], raw_verticesY[MAX_OBJ_NUM], raw_verticesZ[MAX_OBJ_NUM];
float rotated_verticesX[MAX_OBJ_NUM], rotated_verticesY[MAX_OBJ_NUM], rotated_verticesZ[MAX_OBJ_NUM];
int raw_vertices_length;

int zorder_length;
int zorder_index[MAX_OBJ_NUM];
float zorder_distance[MAX_OBJ_NUM];

//********************************
//OBJ formatum kezelesehez
//********************************
float tomb_vertices[MAX_OBJ_NUM][3];
int tomb_faces[MAX_OBJ_NUM][5];
int tomb_vertices_length = 0, tomb_faces_length = 0;

//********************************
//PEGAZUS 3D - INIT
//********************************
void init_3D(void)
{
 rot_degree_x = 0 * Math_PI / 180; rot_degree_x2 = 0;
 rot_degree_y = 0 * Math_PI / 180; rot_degree_y2 = 0;
 rot_degree_z = 0 * Math_PI / 180; rot_degree_z2 = 0;
 raw_vertices_length = 0;
}

//********************************
//OBJ formatum kezelese
//********************************
int getelementcount(unsigned char csv_content[])
{
 int s1, s2;
 for (s1 = s2 = 0; s1 < strlen((const char*)csv_content); ++s1)
 {
  if (csv_content[s1] == 10) break;
  else if (csv_content[s1] == 32) ++s2;
 }
 return s2;
}

void getelement(unsigned char csv_content[], unsigned int data_index, unsigned char csv_content2[])
{
 int s1, s2, s3, s4 = 0;
 for (s1 = 0, s2 = 0; s1 < strlen((const char*)csv_content); ++s1)
 {
  if (csv_content[s1] == 32)
  {
   ++s2;
   if (s2 == data_index)
   {
    for (s3 = s1 + 1; s3 < strlen((const char*)csv_content); ++s3)
    {
     if (csv_content[s3] == 32 || csv_content[s3] == 10)
     {
      csv_content2[s4] = 0;
      return;
     }
     else csv_content2[s4++] = csv_content[s3];
    }
   }
  }
 }
}

void obj_loader(void)
{
 FILE* objfile;
 int i, j;
 float data1, data2, data3;
 unsigned char row1[1024], row2[1024];
 int data_count, max_row_length = 250;
 char tempstr[200];

 objfile = fopen("model.obj", "rt");
 if (objfile == NULL) return;

 tomb_vertices_length = tomb_vertices_length = 0;

 while (!feof(objfile))
 {
  fgets((char*)row1, max_row_length, objfile);

  if (row1[0] == 118 && row1[1] == 32) //*** 'v '
  {
   getelement(row1, 1, row2); data1 = atof((const char*)row2);
   getelement(row1, 2, row2); data2 = atof((const char*)row2);
   getelement(row1, 3, row2); data3 = atof((const char*)row2);
   tomb_vertices[tomb_vertices_length][0] = data1 * 0.5;
   tomb_vertices[tomb_vertices_length][1] = data2 * 0.7;
   tomb_vertices[tomb_vertices_length++][2] = data3 * 0.5;
  }
  else if (row1[0] == 102 && row1[1] == 32) //*** 'f '
  {
   data_count = getelementcount(row1);

   tomb_faces[tomb_faces_length][0] = data_count;
   for (i = 1; i < data_count + 1; ++i)
   {
    getelement(row1, i, row2);
    data1 = atof((const char*)row2);
    tomb_faces[tomb_faces_length][i] = data1 - 1;
   }
   ++tomb_faces_length;
  }
 }
 fclose(objfile);
 int  base_index;
 for (i = 0; i < tomb_faces_length; ++i)
 {
  base_index = tomb_faces[i][1];
  if (tomb_faces[i][0] == 3)
  {
   raw_verticesX[raw_vertices_length] = tomb_vertices[tomb_faces[i][1]][0];
   raw_verticesY[raw_vertices_length] = tomb_vertices[tomb_faces[i][1]][1];
   raw_verticesZ[raw_vertices_length++] = tomb_vertices[tomb_faces[i][1]][2];

   raw_verticesX[raw_vertices_length] = tomb_vertices[tomb_faces[i][2]][0];
   raw_verticesY[raw_vertices_length] = tomb_vertices[tomb_faces[i][2]][1];
   raw_verticesZ[raw_vertices_length++] = tomb_vertices[tomb_faces[i][2]][2];

   raw_verticesX[raw_vertices_length] = tomb_vertices[tomb_faces[i][3]][0];
   raw_verticesY[raw_vertices_length] = tomb_vertices[tomb_faces[i][3]][1];
   raw_verticesZ[raw_vertices_length++] = tomb_vertices[tomb_faces[i][3]][2];
  }
  else if (tomb_faces[i][0] == 4)
  {
   raw_verticesX[raw_vertices_length] = tomb_vertices[tomb_faces[i][1]][0];
   raw_verticesY[raw_vertices_length] = tomb_vertices[tomb_faces[i][1]][1];
   raw_verticesZ[raw_vertices_length++] = tomb_vertices[tomb_faces[i][1]][2];

   raw_verticesX[raw_vertices_length] = tomb_vertices[tomb_faces[i][2]][0];
   raw_verticesY[raw_vertices_length] = tomb_vertices[tomb_faces[i][2]][1];
   raw_verticesZ[raw_vertices_length++] = tomb_vertices[tomb_faces[i][2]][2];

   raw_verticesX[raw_vertices_length] = tomb_vertices[tomb_faces[i][3]][0];
   raw_verticesY[raw_vertices_length] = tomb_vertices[tomb_faces[i][3]][1];
   raw_verticesZ[raw_vertices_length++] = tomb_vertices[tomb_faces[i][3]][2];

   raw_verticesX[raw_vertices_length] = tomb_vertices[tomb_faces[i][1]][0];
   raw_verticesY[raw_vertices_length] = tomb_vertices[tomb_faces[i][1]][1];
   raw_verticesZ[raw_vertices_length++] = tomb_vertices[tomb_faces[i][1]][2];

   raw_verticesX[raw_vertices_length] = tomb_vertices[tomb_faces[i][3]][0];
   raw_verticesY[raw_vertices_length] = tomb_vertices[tomb_faces[i][3]][1];
   raw_verticesZ[raw_vertices_length++] = tomb_vertices[tomb_faces[i][3]][2];

   raw_verticesX[raw_vertices_length] = tomb_vertices[tomb_faces[i][4]][0];
   raw_verticesY[raw_vertices_length] = tomb_vertices[tomb_faces[i][4]][1];
   raw_verticesZ[raw_vertices_length++] = tomb_vertices[tomb_faces[i][4]][2];
  }
 }
}

void rotation_2(int maxitemcount, float* rawarrayX, float* rawarrayY, float* rawarrayZ, float degree_cosx, float degree_sinx, float degree_cosy, float degree_siny, float degree_cosz, float degree_sinz)
{
 int i, drawcolor;
 float t0;
 float rotX[3], rotY[3], rotZ[3];
 int s1;
 float sx = SCREEN_WIDTH / 2;
 float sultra = SCREEN_HEIGHT / 2, sultra2 = SCREEN_HEIGHT / 3;
 int x_minusz_edge = 0, y_minusz_edge = 0, x_max_edge = SCREEN_WIDTH - 1, y_max_edge = SCREEN_HEIGHT - 1;
 float distance;

 pRT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
 pRT->BeginDraw();
 pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
 drawcolor = RGB(0, 0, 0);
 ID2D1SolidColorBrush* ecset;
 pRT->CreateSolidColorBrush(D2D1::ColorF(drawcolor, 1.0f), &ecset);

 //rotaTion
 for (i = 0; i < maxitemcount; ++i)
 {
  rotY[0] = (rawarrayY[i] * degree_cosx) - (rawarrayZ[i] * degree_sinx);
  rotZ[0] = rawarrayY[i] * degree_sinx + rawarrayZ[i] * degree_cosx;

  rotX[0] = rawarrayX[i] * degree_cosy + rotZ[0] * degree_siny;
  rotZ[0] = -rawarrayX[i] * degree_siny + rotZ[0] * degree_cosy;// +

  t0 = rotX[0];
  //Nemi finomhangolas OBJ modellek eseten: "+ (SCREEN_WIDTH / 4)" and "+ (SCREEN_HEIGHT / 4)"
  rotX[0] = t0 * degree_cosz - rotY[0] * degree_sinz;
  rotY[0] = t0 * degree_sinz + rotY[0] * degree_cosz;

  ++i;
  rotY[1] = (rawarrayY[i] * degree_cosx) - (rawarrayZ[i] * degree_sinx);
  rotZ[1] = rawarrayY[i] * degree_sinx + rawarrayZ[i] * degree_cosx;

  rotX[1] = rawarrayX[i] * degree_cosy + rotZ[1] * degree_siny;
  rotZ[1] = -rawarrayX[i] * degree_siny + rotZ[1] * degree_cosy;// +

  t0 = rotX[1];
  //Nemi finomhangolas OBJ modellek eseten: "+ (SCREEN_WIDTH / 4)" and "+ (SCREEN_HEIGHT / 4)"
  rotX[1] = t0 * degree_cosz - rotY[1] * degree_sinz;
  rotY[1] = t0 * degree_sinz + rotY[1] * degree_cosz;

  ++i;
  rotY[2] = (rawarrayY[i] * degree_cosx) - (rawarrayZ[i] * degree_sinx);
  rotZ[2] = rawarrayY[i] * degree_sinx + rawarrayZ[i] * degree_cosx;

  rotX[2] = rawarrayX[i] * degree_cosy + rotZ[2] * degree_siny;
  rotZ[2] = -rawarrayX[i] * degree_siny + rotZ[2] * degree_cosy;// +

  t0 = rotX[2];
  //Nemi finomhangolas OBJ modellek eseten: "+ (SCREEN_WIDTH / 4)" and "+ (SCREEN_HEIGHT / 4)"
  rotX[2] = t0 * degree_cosz - rotY[2] * degree_sinz;
  rotY[2] = t0 * degree_sinz + rotY[2] * degree_cosz;

  //persspektiva projekcio
  rotX[0] = rotX[0] + 300;
  rotY[0] = rotY[0] + 400;
  rotX[1] = rotX[1] + 300;
  rotY[1] = rotY[1] + 400;
  rotX[2] = rotX[2] + 300;
  rotY[2] = rotY[2] + 400;


  //csak drotvazmodell
  pRT->DrawLine(
   D2D1::Point2F(rotX[0], rotY[0]),
   D2D1::Point2F(rotX[1], rotY[1]),
   ecset,
   1.0f);
  pRT->DrawLine(
   D2D1::Point2F(rotX[1], rotY[1]),
   D2D1::Point2F(rotX[2], rotY[2]),
   ecset,
   1.0f);
  pRT->DrawLine(
   D2D1::Point2F(rotX[2], rotY[2]),
   D2D1::Point2F(rotX[0], rotY[0]),
   ecset,
   1.0f);

 }
 pRT->EndDraw();
}

void render_scene(void)
{
 rot_degree_x = rot_degree_x2 * Math_PI / 180;
 rot_degree_y = rot_degree_y2 * Math_PI / 180;
 rot_degree_z = rot_degree_z2 * Math_PI / 180;
 float degree_sinx = sin(rot_degree_x);
 float degree_cosx = cos(rot_degree_x);
 float degree_siny = sin(rot_degree_y);
 float degree_cosy = cos(rot_degree_y);
 float degree_sinz = sin(rot_degree_z);
 float degree_cosz = cos(rot_degree_z);

 rotation_2(raw_vertices_length, raw_verticesX, raw_verticesY, raw_verticesZ, degree_cosx, degree_sinx, degree_cosy, degree_siny, degree_cosz, degree_sinz);
}
