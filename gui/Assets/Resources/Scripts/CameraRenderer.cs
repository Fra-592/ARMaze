﻿using System;
using UnityEngine;
using Wrapper;

public class CameraRenderer : MonoBehaviour {
    private byte[] frame;
    private CameraWrapper camera;
    private Texture2D texture = null;

    private bool m_FinishedBuild = false;
	// Use this for initialization
	void Start() {
        this.camera = CameraWrapper.GetInstance();
        /*        this.camera = CameraWrapper.GetInstance();
                this.camera.InitCamera();
                this.camera.OpenVideoStream(0);
                this.frame = this.camera.GetCameraFrame();
                this.texture = new Texture2D(camera.GetFrameWidth(), camera.GetFrameHeight(), TextureFormat.BGRA32, false);
                this.transform.localScale = new Vector3(camera.GetFrameWidth(), camera.GetFrameHeight(), 1);

                Camera.main.transform.position = this.transform.position;
                Camera.main.transform.Translate(Vector3.back);
                Camera.main.orthographicSize = camera.GetFrameHeight() / 2f + 2;

                this.camera.InitCore();*/
        // this.camera.DisplayCameraStream();
        this.camera.InitCore();
    }
	
	// Update is called once per frame
	void Update() {
        //this.frame = this.camera.GetCameraFrame();

        this.camera.Video();
        if (!m_FinishedBuild) {
            this.camera.Build();

            if (this.camera.CheckBuid()) {
                m_FinishedBuild = true;
                this.camera.InitTrasform();
            }
        }
        else {
//            do {
            this.camera.Tracking();
            this.camera.UpdateTranform();

            double[,] rot = this.camera.GetDeltaRot();
            for (int i = 0 ; i < 3 ; i++) {
                for (int j = 0 ; j < 3 ; j++) {
                    Debug.Log(rot[i, j] + " ");
                }
                Debug.Log("\n");
            }
//            } while (this.camera.CheckTracking());

        }

        this.texture.LoadRawTextureData(this.frame);
        this.texture.Apply();
        this.GetComponent<Renderer>().material.SetTexture("_MainTex", this.texture);
    }
}
