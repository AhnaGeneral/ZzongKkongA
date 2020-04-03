
using System.IO;


using UnityEditor;


using UnityEngine;


 


public class ExportSplatmap : MonoBehaviour


{


    [MenuItem("Terrain/Export Splatmap...")]


    static void Export()


    {


         Terrain terrain = Selection.activeObject as Terrain;


        if (!terrain)


        {


            terrain = Terrain.activeTerrain;


            if (!terrain)


            {


                Debug.Log("Could not find any terrain. Please select or create a terrain first.");


                return;


            }


      }


 


        string path = EditorUtility.SaveFolderPanel("Choose a directory to save the alpha maps:", "", "");


 


        if (path != null && path.Length != 0)


        {


            path = path.Replace(Application.dataPath, "Assets");


            TerrainData terrainData = terrain.terrainData;


            int alphaMapsCount = terrainData.alphamapTextureCount;


 


            for (int i = 0; i < alphaMapsCount; i++)


            {


                Texture2D tex = terrainData.GetAlphamapTexture(i);


                byte[] pngData = tex.EncodeToPNG();


                if (pngData != null)


                {


                    File.WriteAllBytes(path + "/" + tex.name + ".png", pngData);


                }


                else


                {


                    Debug.Log("Could not convert " + tex.name + " to png. Skipping saving texture.");


                }


            }


        }


    }


}
