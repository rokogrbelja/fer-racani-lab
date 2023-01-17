using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ParallaxController : MonoBehaviour
{
    private float speed = 3;
    private float length, startPos;
    public GameObject cam;
    public float parallaxEffect;

    // Start is called before the first frame update
    void Start()
    {
        startPos = transform.position.x;
        length = GetComponent<SpriteRenderer>().bounds.size.x;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        //float dist = cam.transform.position.x * parallaxEffect;
        //transform.position = new Vector3(startPos + dist, transform.position.y, transform.position.z);
        transform.Translate(new Vector3(-speed * Time.deltaTime * parallaxEffect, transform.position.y, transform.position.z));

        // If background moves left by its repeat width, move it back to start position
        if (transform.position.x < startPos - length)
        {
            transform.position = new Vector3(startPos, transform.position.y, transform.position.z);
        }
    }
}
