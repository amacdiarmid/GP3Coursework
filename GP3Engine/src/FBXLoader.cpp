#include "FBXLoader.h"

int level = 0;

void PrintTabs()
{
	for (int i = 0; i < level; i++)
	{
		printf("\t");
	}
}

//return a string based representation based on the attribute type
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type)
{
	switch (type)
	{
	case fbxsdk::FbxNodeAttribute::eUnknown:
		return "eUnknown";
		break;
	case fbxsdk::FbxNodeAttribute::eNull:
		return "eNull";
		break;
	case fbxsdk::FbxNodeAttribute::eMarker:
		return "eMarker";
		break;
	case fbxsdk::FbxNodeAttribute::eSkeleton:
		return "eSkeleton";
		break;
	case fbxsdk::FbxNodeAttribute::eMesh:
		return "eMesh";
		break;
	case fbxsdk::FbxNodeAttribute::eNurbs:
		return "eNurbs";
		break;
	case fbxsdk::FbxNodeAttribute::ePatch:
		return "ePatch";
		break;
	case fbxsdk::FbxNodeAttribute::eCamera:
		return "eCamera";
		break;
	case fbxsdk::FbxNodeAttribute::eCameraStereo:
		return "eCameraStereo";
		break;
	case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
		return "eCameraSwitcher";
		break;
	case fbxsdk::FbxNodeAttribute::eLight:
		return "eLight";
		break;
	case fbxsdk::FbxNodeAttribute::eOpticalReference:
		return "eOpticalReference";
		break;
	case fbxsdk::FbxNodeAttribute::eOpticalMarker:
		return "eOpticalMarker";
		break;
	case fbxsdk::FbxNodeAttribute::eNurbsCurve:
		return "eNurbsCurve";
		break;
	case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
		return "eTrimNurbsSurface";
		break;
	case fbxsdk::FbxNodeAttribute::eBoundary:
		return "eBoundary";
		break;
	case fbxsdk::FbxNodeAttribute::eNurbsSurface:
		return "eNurbsSurface";
		break;
	case fbxsdk::FbxNodeAttribute::eShape:
		return "eShape";
		break;
	case fbxsdk::FbxNodeAttribute::eLODGroup:
		return "eLODGroup";
		break;
	case fbxsdk::FbxNodeAttribute::eSubDiv:
		return "eSubDiv";
		break;
	case fbxsdk::FbxNodeAttribute::eCachedEffect:
		return "eCachedEffect";
		break;
	case fbxsdk::FbxNodeAttribute::eLine:
		return "eLine";
		break;
	default:
		return "unknown";
		break;
	}
}

bool loadFBXFromFile(const string& filename, MeshData *meshData)
{
	level = 0;
	//init sdk manager. this handles memory management
	FbxManager *iSDKManager = FbxManager::Create();


	//create the IO settings object
	FbxIOSettings *IOS = FbxIOSettings::Create(iSDKManager, IOSROOT);
	iSDKManager->SetIOSettings(IOS);

	//create an importer using the sdk manager
	FbxImporter* iImporter = FbxImporter::Create(iSDKManager, "");

	//call the init method which will load the contents of the FBX file
	if (!iImporter->Initialize(filename.c_str(), -1, iSDKManager->GetIOSettings()))
	{
		return false;
	}

	//create a new scene so that is can be populated by the imported file 
	FbxScene *iScene = FbxScene::Create(iSDKManager, "myScene");
	//import contents of file into the scene.
	iImporter->Import(iScene);

	//force Triangulation
	FbxGeometryConverter iGeomConverter(iSDKManager);
	iGeomConverter.Triangulate(iScene, true);

	//process nodes
	FbxNode *iRootNode = iScene->GetRootNode();
	if (iRootNode)
	{
		cout << "Root Node " << iRootNode->GetName() << endl;
		for (int i = 0; i < iRootNode->GetChildCount(); i++)
		{
			processNode(iRootNode->GetChild(i), meshData);
		}
	}


	//Get the first node in the scene
	//FbxNode* lNodeOfInterest = iScene->GetRootNode()->GetChild(0);
	//Vertex *pVerts = new Vertex[1];
	//int pIndices = 0;
	//if (lNodeOfInterest)
	//{
	//	FbxMesh* lMeshOFInterest = lNodeOfInterest->GetMesh();
	//	if (lMeshOFInterest)
	//	{
	//		//first, load the UV information and display them
	//		processMeshTextureCoords(lMeshOFInterest,pVerts, pIndices);
	//		//then, modify certain uv set and save it
	//		//SaveUVInformation(lMeshOFInterest);
	//		//save the modified scene to file
	//		//SaveScene(lSdkManager, lScene, sOutputFile);
	//	}
	//}
	GLenum err = GL_NO_ERROR;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		//Process/log the error.
		cout << "error in loading FBX " << err << endl;
	}

	iImporter->Destroy();
	return true;

}

void processNode(FbxNode *node, MeshData *meshData)
{
	PrintTabs();
	const char *nodeName = node->GetName();
	FbxDouble3 translation = node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	FbxDouble3 scaling = node->LclScaling.Get();

	cout << "Node " << nodeName
		<< " Position " << translation[0] << " " << translation[1] << " " << translation[2] << " "
		<< " Rotation " << rotation[0] << " " << rotation[1] << " " << rotation[2] << " "
		<< " Scale " << scaling[0] << " " << scaling[1] << " " << scaling[2] << endl;

	level++;
	//print the nodes attributes
	for (int i = 0; i < node->GetNodeAttributeCount(); i++)
	{
		processAttribute(node->GetNodeAttributeByIndex(i), meshData);
	}

	//recursivly print the children
	for (int j = 0; j < node->GetChildCount(); j++)
	{
		processNode(node->GetChild(j), meshData);
	}
	level--;
	PrintTabs();
}

void processAttribute(FbxNodeAttribute *attribute, MeshData *meshData)
{
	if (!attribute)
	{
		return;
	}
	FbxString typeName = GetAttributeTypeName(attribute->GetAttributeType());
	FbxString attrName = attribute->GetName();
	PrintTabs();
	cout << "attribute " << typeName.Buffer() << " name " << attrName << endl;
	switch (attribute->GetAttributeType())
	{
	case FbxNodeAttribute::eMesh:
		processMesh(attribute->GetNode()->GetMesh(), meshData);
	case FbxNodeAttribute::eCamera:
		return;
	case FbxNodeAttribute::eLight:
		return;
	default:
		break;
	}
}

void processMesh(FbxMesh *mesh, MeshData *meshData)
{
	int numVerts = mesh->GetControlPointsCount();
	int numIndices = mesh->GetPolygonVertexCount();

	Vertex *pVerts = new Vertex[numVerts];
	int *pIndices = mesh->GetPolygonVertices();

	for (int i = 0; i < numVerts; i++)
	{
		FbxVector4 currentVert = mesh->GetControlPointAt(i);
		pVerts[i].position = vec3(currentVert[0], currentVert[1], currentVert[2]);
		pVerts[i].colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//pVerts[i].texCoords = vec2(1.0f, 1.0f);
	}

	processMeshTextureCoords(mesh, pVerts, numVerts);
	processMeshNormals(mesh, pVerts, numVerts);

	uint intVertCount = meshData->vertices.size();
	for (int i = 0; i < numVerts; i++)
	{
		meshData->vertices.push_back(pVerts[i]);
	}
	for (int i = 0; i < numIndices; i++)
	{
		meshData->indices.push_back(intVertCount + pIndices[i]);
	}
	cout << "Vertices " << numVerts << " indices " << numIndices << endl;

	if (pVerts)
	{
		delete[] pVerts;
		pVerts = NULL;
	}
}	

void processMeshTextureCoords(FbxMesh *mesh, Vertex *verts, int numVerts)
{
	FbxLayerElementUV *fbxLayerUV = mesh->GetLayer(0)->GetUVs();

	for (int iPolygon = 0; iPolygon < mesh->GetPolygonCount(); iPolygon++)
	{
		for (unsigned iPolygonVertex = 0; iPolygonVertex < 3; iPolygonVertex++)
		{
			int fbxCornerIndex = mesh->GetPolygonVertex(iPolygon, iPolygonVertex);
			FbxVector2 fbxUV = FbxVector2(0.0, 0.0);

			//get tex coords
			if (fbxLayerUV)
			{
				int iUVIndex = 0;
				switch (fbxLayerUV->GetMappingMode())
				{
				case FbxLayerElement::eByControlPoint:
					iUVIndex = fbxCornerIndex;
					break;
				case FbxLayerElement::eByPolygonVertex:
					iUVIndex = mesh->GetTextureUVIndex(iPolygon, iPolygonVertex, FbxLayerElement::eTextureDiffuse);

					break;
				case FbxLayerElement::eByPolygon:
					iUVIndex = iPolygon;
					break;
				default:
					break;
				}

				fbxUV = fbxLayerUV->GetDirectArray().GetAt(iUVIndex);
				verts[fbxCornerIndex].texCoords.x = fbxUV[0];
				verts[fbxCornerIndex].texCoords.y = 1.0f - fbxUV[1];
			}
		}
	}

	//get all UV set names
	//FbxStringList lUVSetNameList;
	//mesh->GetUVSetNames(lUVSetNameList);
	////iterating over all uv sets
	//for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
	//{
	//	//get lUVSetIndex-th uv set
	//	const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
	//	const FbxGeometryElementUV* lUVElement = mesh->GetElementUV(lUVSetName);
	//	if (!lUVElement)
	//		continue;
	//	// only support mapping mode eByPolygonVertex and eByControlPoint
	//	if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
	//		lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
	//		return;
	//	//index array, where holds the index referenced to the uv data
	//	const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
	//	const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;
	//	//iterating through the data by polygon
	//	const int lPolyCount = mesh->GetPolygonCount();
	//	if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	//	{
	//		for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
	//		{
	//			// build the max index array that we need to pass into MakePoly
	//			const int lPolySize = mesh->GetPolygonSize(lPolyIndex);
	//			for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
	//			{
	//				FbxVector2 lUVValue;
	//				//get the index of the current vertex in control points array
	//				int lPolyVertIndex = mesh->GetPolygonVertex(lPolyIndex, lVertIndex);
	//				//the UV index depends on the reference mode
	//				int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;
	//				lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
	//				
	//				verts[lVertIndex].texCoords.x = lUVValue[0];
	//				verts[lVertIndex].texCoords.y = 1.0f - lUVValue[1];
	//				if (lVertIndex == numVerts)
	//				{
	//					return;
	//				}
	//			}
	//		}
	//	}
	//	else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	//	{
	//		int lPolyIndexCounter = 0;
	//		for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
	//		{
	//			// build the max index array that we need to pass into MakePoly
	//			const int lPolySize = mesh->GetPolygonSize(lPolyIndex);
	//			for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
	//			{
	//				if (lPolyIndexCounter < lIndexCount)
	//				{
	//					FbxVector2 lUVValue;
	//					//the UV index depends on the reference mode
	//					int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;
	//					lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
	//					
	//					verts[lPolyIndexCounter].texCoords.x = lUVValue[0];
	//					verts[lPolyIndexCounter].texCoords.y = 1.0f - lUVValue[1];
	//					lPolyIndexCounter++;
	//					if (lPolyIndexCounter == numVerts)
	//					{
	//						return;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

void processMeshNormals(FbxMesh *mesh, Vertex *verts, int numVerts)
{
	for (int iPolygon = 0; iPolygon < mesh->GetPolygonCount(); iPolygon++)
	{
		for (unsigned iPolygonVertex = 0; iPolygonVertex < 3; iPolygonVertex++)
		{
			int fbxCornerIndex = mesh->GetPolygonVertex(iPolygon, iPolygonVertex);
			FbxVector4 fbxNormal;
			mesh->GetPolygonVertexNormal(iPolygon, iPolygonVertex, fbxNormal);
			fbxNormal.Normalize();
			verts[fbxCornerIndex].normal.x = fbxNormal[0];
			verts[fbxCornerIndex].normal.y = fbxNormal[1];
			verts[fbxCornerIndex].normal.z = fbxNormal[2];
		}
	}
}