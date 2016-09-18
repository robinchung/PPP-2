/********************************************************************************
*																				*
*	Description:	be used for load 3d object model to OpenGL					*
*	Author	   :	Yingmou.Zhong												*
*	Date	   :    2016-06-29													*
*																				*
*********************************************************************************/
#pragma once

#include <vector>
#include <map>
#include "TPOpenGL.h"
typedef struct
{
	float x;
	float y;
	float z;
}tVert;

typedef struct 
{
	float u;
	float v;
}tTex;

//��ʶ��������Ľṹ��
typedef struct tagVertInfo
{
	tagVertInfo(const tVert &pos, const tTex &tc, const tVert &norm)
	{ vPos = pos; vTexcoord = tc; vNormal = norm; }
	tVert  vPos;
	tTex vTexcoord;
	tVert  vNormal;
}tVertInfo;

typedef struct tagMaterialInfo
{
	char TextureName[20];
	GLfloat crAmbient[4];
	GLfloat crDiffuse[4];
	GLfloat crSpecular[4];
	GLfloat fShiness;
	GLuint nDiffuseMap;
	GLuint nSpecularMap;
	GLuint nBumpMap;
	GLuint TexObjDiffuseMap;
	GLuint TexObjSpecularMap;
	GLuint TexObjBumpMap;
}tMaterialInfo;

typedef struct tag3DObject
{
	int nMaterialID;
	bool bHasTexture;
	bool bHasNormal;
	std::vector<tVert> PosVerts;
	std::vector<tVert> Normals;
	std::vector<tTex> Texcoords;
	std::vector<unsigned short> Indexes;
	unsigned int nNumIndexes;
	GLuint nPosVBO;
	GLuint nNormVBO;
	GLuint nTexcoordVBO;
	GLuint nIndexVBO;
}t3DObject;

typedef struct tag3DModel
{
	bool bIsTextured;
	std::vector<tMaterialInfo> tMatInfoVec;
	std::vector<t3DObject>     t3DObjVec;
}t3DModel;

class ObjModelOpenGL
{
public:
	ObjModelOpenGL(void);
	~ObjModelOpenGL(void);
	void Import3DObjectFile(char *ObjFilePath, GLuint usage = GL_STREAM_DRAW);//usage ����ʾ�û������򽫻ᱻ���ʹ�ã�
																			  //������ҪĿ����������ʾOpenGL�öԸû������������̶ֳȵ��Ż�
	void DrawModel();
	void CleanObjData();
private:
	void Load3DObjectFile(char *ObjFilePath);
	void ProcessVertexInfo(char *str);
	void ProcessFaceInfo(t3DModel *pModel,char *str);
	void ProcessMtlFileInfo(t3DModel *pModel,char *mtlFilePath);
	void ComputeNormals(t3DObject *t3DObj);
private:
	FILE *pObjFile,*pMtlFile;
	t3DModel m_ModelObj;
	int  m_nCurObjectCount;//���ڼ�¼������t3DObject����������t3DModel
	int  m_nCurMaterialCount;
	// ����λ������Ⱥ
	std::vector<tVert>   m_VPositionVec;
	// ������������Ⱥ
	std::vector<tTex>  m_VTexcoordVec;
	// ���㷨������Ⱥ
	std::vector<tVert>  m_VNormalVec;
	std::map<tVertInfo, unsigned short> m_VObjectIndexMap;
};
