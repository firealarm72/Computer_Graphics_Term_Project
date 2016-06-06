using UnityEngine;
using System.Collections;

public class GroundScript : MonoBehaviour {
    //Material texture offset rate
    float speed = 1.2f;

    //Offset the material texture at a constant rate
    void Update()
    {
        float offset = Time.time * speed;
        GetComponent<Renderer>().material.mainTextureOffset = new Vector2(0, -offset);
    }
    // Use this for initialization
    void Start () {
	
	}
	
	
}
