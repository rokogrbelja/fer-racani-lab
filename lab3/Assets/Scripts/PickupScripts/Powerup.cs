using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Powerup : MonoBehaviour
{
    protected GameObject player;
    protected GameManager game;

    // Start is called before the first frame update
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        game = GameObject.FindGameObjectWithTag("GameManager").GetComponent<GameManager>();
    }

    // Update is called once per frame
    void Update()
    {
        DestroyOutOfBounds();
    }

    public void DestroyOutOfBounds()
    {
        if (transform.position.x < game.leftBound - 3) Destroy(gameObject);
    }

    public void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Player"))
        {
            OnPickup();
            Destroy(gameObject);
        }
    }

    public virtual void OnPickup() { }
}
