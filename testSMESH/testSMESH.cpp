// testSMESH.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include "SMESH_Gen.hxx"
#include "BRep_Tool.hxx"
#include "BRepTools.hxx" 
#include "TopoDS_Shape.hxx"
#include "TopoDS.hxx"
#include "TopExp_Explorer.hxx"
#include "BRepPrimAPI_MakeBox.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "GProp_GProps.hxx"
#include "BRepGProp.hxx"
#include"TopTools_ListOfShape.hxx"
#include "StdMeshers_Regular_1D.hxx"
#include "StdMeshers_LocalLength.hxx"
#include "StdMeshers_Quadrangle_2D.hxx"
#include "StdMeshers_NumberOfSegments.hxx"
#include "StdMeshers_Hexa_3D.hxx"
#include "StdMeshers_Projection_3D.hxx"
#include "StdMeshers_MaxLength.hxx"
#include "StdMeshers_MEFISTO_2D.hxx"
#include "NETGENPlugin_NETGEN_3D.hxx"
#include "SMESHDS_GroupOnGeom.hxx"
int _tmain(int argc, _TCHAR* argv[])
{
	TopoDS_Shape myShape = BRepPrimAPI_MakeBox (30.,20.,10.);
	
	GProp_GProps System;
    BRepGProp::VolumeProperties(myShape,System);
    Standard_Real Volume = System.Mass();
    cout<<"The mass of the volume is :"<<Volume<<endl;

	SMESH_Gen gen=SMESH_Gen();
	
	SMESH_Mesh *mesh = gen.CreateMesh(1, false);
	mesh->Clear();
	mesh->ShapeToMesh(myShape);	

	/*
	StdMeshers_MaxLength *hyp1D=new StdMeshers_MaxLength(0,1,&gen);
	hyp1D->SetLength(2.0);
	mesh->AddHypothesis(myShape,0);
		

	StdMeshers_Regular_1D *al1D=new StdMeshers_Regular_1D(1,1,&gen);
	mesh->AddHypothesis(myShape,1);

	
	StdMeshers_MEFISTO_2D *al2D=new StdMeshers_MEFISTO_2D(2,1,&gen);
	mesh->AddHypothesis(myShape,2);

	NETGENPlugin_NETGEN_3D alg3D(3,1,&gen);
	mesh->AddHypothesis(myShape,3);
	*/
	
	
	
	StdMeshers_LocalLength *hyp1D=new StdMeshers_LocalLength(0,1,&gen);
	hyp1D->SetLength(1.0);
	hyp1D->SetPrecision(1.0-7); 
	mesh->AddHypothesis(myShape,0);

	StdMeshers_Regular_1D *al1D=new StdMeshers_Regular_1D(1,1,&gen);
	mesh->AddHypothesis(myShape,1);

	StdMeshers_Quadrangle_2D *al2D=new StdMeshers_Quadrangle_2D(2,1,&gen);
	mesh->AddHypothesis(myShape,2);	
	
	StdMeshers_Hexa_3D al3D(3,1,&gen);	
	mesh->AddHypothesis(myShape,3);
  
	
    if(!gen.CheckAlgoState(*mesh,myShape))
	{
			cout<<"Algorithm not properly set!"<<endl;
			return 1;
	}
	
	cout<<"Algorithm properly set.\nBegin meshing..."<<endl;
	//getchar();

	if(gen.Compute(*mesh,myShape))
	{
	cout<<"Meshing Done."<<endl;
	cout<<"Number of nodes:"<<mesh->NbNodes()<<endl;
	
	}
   //add groups from geometry
	if(mesh->NbNodes()>0)
	{
	cout<<"Adding a group from a geometry."<<endl;
	TopExp_Explorer ex(myShape,TopAbs_FACE);
	int idGroup=0;
	char name[20];
	while(ex.More())
	{
		
		sprintf(name,"group%d",idGroup);
		mesh->AddGroup(SMDSAbs_Face,name,idGroup,TopoDS::Face(ex.Current()));
		idGroup++;
		ex.Next();
	}
	 cout<<"Export mesh."<<endl;
 	 mesh->ExportUNV("test.unv");

	
	}
	
	cout<<"Press ANY key to exit."<<endl;
	getchar();

	

	return 0;
}

