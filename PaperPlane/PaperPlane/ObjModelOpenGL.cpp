/********************************************************************************
*																				*
*	Description:	be used for load 3d object model to OpenGL					*
*	Author	   :	Yingmou.Zhong												*
*	Date	   :    2016-06-29													*
*																				*
*********************************************************************************/
#include "StdAfx.h"
#include "ObjModelOpenGL.h"

#define MAX_STRINGPERLINE 255
#define TEMPBUFFER 20
#define VERTEX_OF_FACE     3
ObjModelOpenGL::ObjModelOpenGL(void)
{
	 m_nCurObjectCount = 0;
	 m_nCurMaterialCount = 0;
}

ObjModelOpenGL::~ObjModelOpenGL(void)
{
	for(unsigned int i = 0; i < m_ModelObj.t3DObjVec.size(); i++) 
	{
		glDeleteBuffers(1, &m_ModelObj.t3DObjVec[i].nPosVBO);
		glDeleteBuffers(1, &m_ModelObj.t3DObjVec[i].nNormVBO);
		glDeleteBuffers(1, &m_ModelObj.t3DObjVec[i].nTexcoordVBO);
		glDeleteBuffers(1, &m_ModelObj.t3DObjVec[i].nIndexVBO);
	}
	m_ModelObj.t3DObjVec.clear();
	m_ModelObj.tMatInfoVec.clear();
}

void ObjModelOpenGL::ProcessVertexInfo(char *str)
{
	//char StrBuffer[MAX_STRINGPERLINE] = {0};

	if(' '== str[1])			// ����ǿգ�����Ƕ���("v")
	{
		tVert  vNewVertex;
		char x[TEMPBUFFER],y[TEMPBUFFER],z[TEMPBUFFER];
		// ���붥�����꣬��ʽ��"v x y z"
		sscanf(str,"%*s%s%s%s",x,y,z);
		vNewVertex.x = atof(x);
		vNewVertex.y = atof(y);
		vNewVertex.z = atof(z);
		// ���һ���µĶ��㵽�����б���
		m_VPositionVec.push_back(vNewVertex);
	}
	else if('t' == str[1])			// �����'t'����ض�����������("vt")
	{
		tTex vNewTexCoord;

		// �����������꣬��ʽ��"vt u v"
		char u[TEMPBUFFER],v[TEMPBUFFER];
		sscanf(str,"%*s%s%s",u,v);
		vNewTexCoord.u = atof(u);
		vNewTexCoord.v = atof(v);
		// ���һ���µ��������굽�б���
		m_VTexcoordVec.push_back(vNewTexCoord);
	}
	else if('n' == str[1])		// ��������Ƿ�����("vn")
	{
		tVert  vNewNormal;

		// ���뷨�ߣ���ʽ��"vn x y z"
		//fscanf_s(pFile, "%f %f %f", &vNewNormal.x, &vNewNormal.y, &vNewNormal.z);
		char vx[TEMPBUFFER],vy[TEMPBUFFER],vz[TEMPBUFFER];
		sscanf(str,"%*s%s%s%s",vx,vy,vz);
		vNewNormal.x = atof(vx);
		vNewNormal.y = atof(vy);
		vNewNormal.z = atof(vz);
		// ���һ���µ��������굽�б���
		m_VNormalVec.push_back(vNewNormal);
	}

}

bool operator<(const tVertInfo &vertInfo1,const tVertInfo &vertInfo2)
{
	if(vertInfo1.vPos.x != vertInfo2.vPos.x)
	{
		return vertInfo1.vPos.x < vertInfo2.vPos.x;
	}
	else if(vertInfo1.vPos.y != vertInfo2.vPos.y)
	{
		return vertInfo1.vPos.y < vertInfo2.vPos.y;
	}
	else if(vertInfo1.vPos.z != vertInfo2.vPos.z)
	{
		return vertInfo1.vPos.z < vertInfo2.vPos.z;
	}
	else if(vertInfo1.vTexcoord.u != vertInfo2.vTexcoord.u)
	{
		return vertInfo1.vTexcoord.u < vertInfo2.vTexcoord.u;
	}
	else if(vertInfo1.vTexcoord.v != vertInfo2.vTexcoord.v)
	{
		return vertInfo1.vTexcoord.v < vertInfo2.vTexcoord.v;
	}
	if(vertInfo1.vNormal.x != vertInfo2.vNormal.x)
	{
		return vertInfo1.vNormal.x < vertInfo2.vNormal.x;
	}
	else if(vertInfo1.vNormal.y != vertInfo2.vNormal.y)
	{
		return vertInfo1.vNormal.y < vertInfo2.vNormal.y;
	}
	else
	{
		return vertInfo1.vNormal.z < vertInfo2.vNormal.z;
	}
}

void ObjModelOpenGL::ProcessFaceInfo(t3DModel *pModel,char *str)
{
	if(m_nCurObjectCount <= 0)
	{
		return;
	}
	char TemBuf[TEMPBUFFER];
	sscanf(str, "%*s%s",TemBuf);
	char FirstBuf[TEMPBUFFER],SecondBuf[TEMPBUFFER],ThirdBuf[TEMPBUFFER];
	int i = sscanf(str, "%*s%s%s%s",FirstBuf,SecondBuf,ThirdBuf);//��һ���з�Ϊ�����ַ���
	std::string vtn[3];//���Ա��������ַ���Ƭ��
	vtn[0].assign(FirstBuf);
	vtn[1].assign(SecondBuf);
	vtn[2].assign(ThirdBuf);
	t3DObject *pCurObj = &pModel->t3DObjVec[m_nCurObjectCount - 1];
	unsigned int vIdx = 0, tIdx = 0, nIdx = 0;//���㡢�������ߵ�������
	tVert  vPosVert;
	tTex vTexcoord;
	tVert  vNormal={0, 1, 0};
	size_t   nDistance = 0;
	if(strstr(TemBuf,"//"))//��ʽΪv//n
	{
		if(!pCurObj->bHasNormal)
		{
			pCurObj->bHasNormal = true;
		}
		for(int count0=0;count0<3;count0++)
		{	
			strcpy(TemBuf,vtn[count0].c_str());
			sscanf(TemBuf, "%d//%d", &vIdx,&nIdx);
			std::map<tVertInfo, unsigned short>::iterator pFindPos 
				= m_VObjectIndexMap.find(tVertInfo(m_VPositionVec[vIdx - 1], vTexcoord, m_VNormalVec[nIdx - 1]));
			if(m_VObjectIndexMap.end() != pFindPos)
			{
				pCurObj->Indexes.push_back(pFindPos->second);
			}
			else
			{
				pCurObj->PosVerts.push_back(m_VPositionVec[vIdx - 1]);
				pCurObj->Texcoords.push_back(vTexcoord);
				pCurObj->Normals.push_back(m_VNormalVec[nIdx - 1]);
				pCurObj->Indexes.push_back(pCurObj->PosVerts.size() - 1);
				m_VObjectIndexMap.insert(std::pair<tVertInfo, unsigned short>(
					tVertInfo(m_VPositionVec[vIdx - 1], vTexcoord, m_VNormalVec[nIdx - 1]), 
					pCurObj->PosVerts.size() - 1));
			}
		}
	}
	else if(3 == sscanf(TemBuf, "%d/%d/%d", &vIdx, &tIdx, &nIdx)) //  ��ʽv/t/n���˴��߼�����Ҫ����
	{
		if(!pCurObj->bHasTexture)
		{
			pCurObj->bHasTexture = true;
		}
		if(!pCurObj->bHasNormal)
		{
			pCurObj->bHasNormal = true;
		}
		for(int count1=0;count1<3;count1++)
		{	
			strcpy(TemBuf,vtn[count1].c_str());
			sscanf(TemBuf, "%d/%d/%d", &vIdx, &tIdx, &nIdx);
			std::map<tVertInfo, unsigned short>::iterator pFindPos 
				= m_VObjectIndexMap.find(tVertInfo(m_VPositionVec[vIdx - 1], m_VTexcoordVec[tIdx - 1], m_VNormalVec[nIdx - 1]));
			if(m_VObjectIndexMap.end() != pFindPos)
			{
				pCurObj->Indexes.push_back(pFindPos->second);
			}
			else
			{
				pCurObj->PosVerts.push_back(m_VPositionVec[vIdx - 1]);
				pCurObj->Texcoords.push_back(m_VTexcoordVec[tIdx - 1]);
				pCurObj->Normals.push_back(m_VNormalVec[nIdx - 1]);
				pCurObj->Indexes.push_back(pCurObj->PosVerts.size() - 1);
				m_VObjectIndexMap.insert(std::pair<tVertInfo, unsigned short>(
					tVertInfo(m_VPositionVec[vIdx - 1], m_VTexcoordVec[tIdx - 1], m_VNormalVec[nIdx - 1]), 
					pCurObj->PosVerts.size() - 1));
			}
		}
	}
	else if(2 == sscanf_s(TemBuf, "%d/%d", &vIdx, &tIdx)) //  ��ʽv/t
	{
		if(!pCurObj->bHasTexture)
		{
			pCurObj->bHasTexture = true;
		}
		for(int count2=0;count2<3;count2++)
		{	
			strcpy(TemBuf,vtn[count2].c_str());
			sscanf(TemBuf, "%d/%d", &vIdx, &tIdx);
			std::map<tVertInfo, unsigned short>::iterator pFindPos 
				= m_VObjectIndexMap.find(tVertInfo(m_VPositionVec[vIdx - 1], m_VTexcoordVec[tIdx - 1], vNormal));
			if(m_VObjectIndexMap.end() != pFindPos)
			{
				pCurObj->Indexes.push_back(pFindPos->second);
			}
			else
			{
				pCurObj->PosVerts.push_back(m_VPositionVec[vIdx - 1]);
				pCurObj->Texcoords.push_back(m_VTexcoordVec[tIdx - 1]);
				pCurObj->Normals.push_back(vNormal);
				pCurObj->Indexes.push_back(pCurObj->PosVerts.size() - 1);
				m_VObjectIndexMap.insert(std::pair<tVertInfo, unsigned short>(
					tVertInfo(m_VPositionVec[vIdx - 1], m_VTexcoordVec[tIdx - 1],vNormal), 
					pCurObj->PosVerts.size() - 1));
			}
		}
	}
	else//��ʽv
	{
		for(int count3=0;count3<3;count3++)
		{	
			strcpy(TemBuf,vtn[count3].c_str());
			sscanf(TemBuf, "%d", &vIdx);
			std::map<tVertInfo, unsigned short>::iterator pFindPos 
				= m_VObjectIndexMap.find(tVertInfo(m_VPositionVec[vIdx - 1], vTexcoord, vNormal));
			if(m_VObjectIndexMap.end() != pFindPos)
			{
				pCurObj->Indexes.push_back(pFindPos->second);
			}
			else
			{
				pCurObj->PosVerts.push_back(m_VPositionVec[vIdx - 1]);
				pCurObj->Texcoords.push_back(m_VTexcoordVec[tIdx - 1]);
				pCurObj->Normals.push_back(m_VNormalVec[nIdx - 1]);
				pCurObj->Indexes.push_back(pCurObj->PosVerts.size() - 1);
				m_VObjectIndexMap.insert(std::pair<tVertInfo, unsigned short>(
					tVertInfo(m_VPositionVec[vIdx - 1],vTexcoord, vNormal), 
					pCurObj->PosVerts.size() - 1));
			}
		}
	}
}

void ObjModelOpenGL::ProcessMtlFileInfo(t3DModel *pModel,char *mtlFilePath)
{
	char StrBuffer[MAX_STRINGPERLINE] = {0};
	char ChKeyword = 0;
	pMtlFile = fopen(mtlFilePath,"r+");
	if(pMtlFile)
	{
		while(!feof(pMtlFile))
		{
			fgets(StrBuffer, MAX_STRINGPERLINE,pMtlFile);//�˴����ɣ�fgets������һ��ʱ���ָ��buffersize
			ChKeyword = StrBuffer[0];				  //����һ�е�������ֻ������һ�У�ָ��������һ��
			switch(ChKeyword)
			{
			case 'n':
				{

				}
			break;
			case 'K':
			case 'k':
			break;
			case 'T':
			break;
			case 'd':
			break;
			case 'N':
			break;
			case 'm':
			break;
			case 'b':
			break;
			default:
			break;
			}
		}
	}
	fclose(pMtlFile);
}

void ObjModelOpenGL::Load3DObjectFile(char *ObjFilePath)//ע��˴���pModelָ����ȫ��ָ��Ĺ�ϵ
{
	char StrBuffer[MAX_STRINGPERLINE] = {0};
	char ChKeyword = 0;
	pObjFile = fopen(ObjFilePath,"r+");
	if(pObjFile)
	{
		while(!feof(pObjFile))
		{
			fgets(StrBuffer, MAX_STRINGPERLINE,pObjFile);//�˴����ɣ�fgets������һ��ʱ���ָ��buffersize
			ChKeyword = StrBuffer[0];				  //����һ�е�������ֻ������һ�У�ָ��������һ��			
			switch(ChKeyword)
			{
			case 'm':
				{
					ProcessMtlFileInfo(&m_ModelObj,"3DModel/chess.mtl");//�˴���·��д���ˣ��Ժ�Ҫ�ĳɿ��Ի�ȡ·��
				}
			break;
			case 'u':
			break;
			case 'v':
				{	
					ProcessVertexInfo(StrBuffer);
				}
			break;
			case 'f':
				{
					if(0 == m_nCurObjectCount) //����һ���޲������
					{
					t3DObject newObject = {0};
					m_ModelObj.t3DObjVec.push_back(newObject);
					++m_nCurObjectCount;
					}
					ProcessFaceInfo(&m_ModelObj,StrBuffer);
				}
			break;
			default:
			break;
			}
		}
	}
	fclose(pObjFile);
}

void ObjModelOpenGL::ComputeNormals(t3DObject *t3DObj)// �������ڼ������Ķ��㷨����, ����ķ�������Ҫ�����������
{

}

void ObjModelOpenGL::Import3DObjectFile(char *ObjFilePath, GLuint usage)
{
	m_VPositionVec.clear();
	m_VNormalVec.clear();
	m_VTexcoordVec.clear();
	m_VObjectIndexMap.clear();
	Load3DObjectFile(ObjFilePath);
	for(int count=0;count<m_ModelObj.t3DObjVec.size();count++)//��VBO
	{
		if(!m_ModelObj.t3DObjVec[count].PosVerts.empty())
		{
			glGenBuffers(1, &m_ModelObj.t3DObjVec[count].nPosVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_ModelObj.t3DObjVec[count].nPosVBO);
			glBufferData(GL_ARRAY_BUFFER, m_ModelObj.t3DObjVec[count].PosVerts.size() * sizeof(tVert), 
			(GLvoid*)&m_ModelObj.t3DObjVec[count].PosVerts[0], usage);
		}

		if(!m_ModelObj.t3DObjVec[count].bHasNormal)
		{
			// ���㶥��ķ�����
			ComputeNormals(&m_ModelObj.t3DObjVec[count]);//��Ҫ������������գ���ʱ��ûʵ��
			m_ModelObj.t3DObjVec[count].bHasNormal = true;
		}
		
		if(!m_ModelObj.t3DObjVec[count].Normals.empty())
		{
			glGenBuffers(1, &m_ModelObj.t3DObjVec[count].nNormVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_ModelObj.t3DObjVec[count].nNormVBO);
			glBufferData(GL_ARRAY_BUFFER, m_ModelObj.t3DObjVec[count].Normals.size() * sizeof(tVert), 
				(GLvoid*)&m_ModelObj.t3DObjVec[count].Normals[0], usage);
		}

		if(m_ModelObj.t3DObjVec[count].bHasTexture && !m_ModelObj.t3DObjVec[count].Texcoords.empty())
		{
			glGenBuffers(1, &m_ModelObj.t3DObjVec[count].nTexcoordVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_ModelObj.t3DObjVec[count].nTexcoordVBO);
			glBufferData(GL_ARRAY_BUFFER, m_ModelObj.t3DObjVec[count].Texcoords.size() * sizeof(tTex), 
				(GLvoid*)&m_ModelObj.t3DObjVec[count].Texcoords[0], usage);
		}

		if(!m_ModelObj.t3DObjVec[count].Indexes.empty())
		{
			glGenBuffers(1, &m_ModelObj.t3DObjVec[count].nIndexVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ModelObj.t3DObjVec[count].nIndexVBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_ModelObj.t3DObjVec[count].Indexes.size() * sizeof(unsigned short), 
				(GLvoid*)&m_ModelObj.t3DObjVec[count].Indexes[0], usage);

			m_ModelObj.t3DObjVec[count].nNumIndexes = m_ModelObj.t3DObjVec[count].Indexes.size();
		}
	}
}

void ObjModelOpenGL::DrawModel()
{
	for(unsigned int ModelObjectCount=0;ModelObjectCount<m_ModelObj.t3DObjVec.size();ModelObjectCount++)
	{
		t3DObject *pObj = &m_ModelObj.t3DObjVec[ModelObjectCount];
		/*int FaceCount = (pObj->Indexes.size())/3;//�˷���ͼ̫ԭʼ��ʱ����
		for(int counter=0;counter<FaceCount*3;)
		{
			 glBegin(GL_TRIANGLES); 
			 glTexCoord2f((pObj->Texcoords[pObj->Indexes[counter]]).u,(pObj->Texcoords[pObj->Indexes[counter]]).v);
			 glVertex3f((pObj->PosVerts[pObj->Indexes[counter]]).x,(pObj->PosVerts[pObj->Indexes[counter]]).y,(pObj->PosVerts[pObj->Indexes[counter]]).z);
			 glTexCoord2f((pObj->Texcoords[pObj->Indexes[counter+1]]).u,(pObj->Texcoords[pObj->Indexes[counter+1]]).v);
			 glVertex3f((pObj->PosVerts[pObj->Indexes[counter+1]]).x,(pObj->PosVerts[pObj->Indexes[counter+1]]).y,(pObj->PosVerts[pObj->Indexes[counter+1]]).z);
			 glTexCoord2f((pObj->Texcoords[pObj->Indexes[counter+2]]).u,(pObj->Texcoords[pObj->Indexes[counter+2]]).v);
			 glVertex3f((pObj->PosVerts[pObj->Indexes[counter+2]]).x,(pObj->PosVerts[pObj->Indexes[counter+2]]).y,(pObj->PosVerts[pObj->Indexes[counter+2]]).z);
			 glEnd();// �����λ��ƽ���    
			 counter+=3;
		}*/

		glBindBuffer(GL_ARRAY_BUFFER, m_ModelObj.t3DObjVec[ModelObjectCount].nPosVBO);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_ModelObj.t3DObjVec[ModelObjectCount].nNormVBO);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, NULL);
		if(m_ModelObj.t3DObjVec[ModelObjectCount].nTexcoordVBO)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_ModelObj.t3DObjVec[ModelObjectCount].nTexcoordVBO);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ModelObj.t3DObjVec[ModelObjectCount].nIndexVBO);

		glDrawElements(GL_TRIANGLES, m_ModelObj.t3DObjVec[ModelObjectCount].nNumIndexes, GL_UNSIGNED_SHORT, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}
}

void ObjModelOpenGL::CleanObjData()
{
	m_VPositionVec.clear();
	m_VNormalVec.clear();
	m_VTexcoordVec.clear();
	m_VObjectIndexMap.clear();
	for(int count=0;count<m_ModelObj.t3DObjVec.size();count++)
	{
		m_ModelObj.t3DObjVec[count].PosVerts.clear();
		m_ModelObj.t3DObjVec[count].Normals.clear();
		m_ModelObj.t3DObjVec[count].Texcoords.clear();
		m_ModelObj.t3DObjVec[count].Indexes.clear();
	}
}