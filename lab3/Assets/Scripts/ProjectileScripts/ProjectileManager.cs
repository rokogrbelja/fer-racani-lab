using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class ProjectileManager : MonoBehaviour
{
    public GameObject projectile;
    public GameObject hitEffect;

    private AudioSource projectileAudio;
    public AudioClip hitSound;
    public AudioClip fireSound;

    public bool isFriendly;
    public float damage;
    public float speed;
    public Vector3 direction;

    public GameManager game;

    // Start is called before the first frame update
    void Start()
    {
        game = GameObject.FindGameObjectWithTag("GameManager").GetComponent<GameManager>();
        projectileAudio = GetComponent<AudioSource>();
        GetComponent<Rigidbody2D>().velocity = direction * speed;
        projectileAudio.PlayOneShot(fireSound);
    }

    // Update is called once per frame
    void Update()
    {
        DestroyOutOfBounds();
    }

    // Destroy projectiles that leave the view
    void DestroyOutOfBounds()
    {
        if (transform.position.x > game.rightBound + 3 || transform.position.x < game.leftBound - 3
            || transform.position.y > game.upperBound + 1 || transform.position.y < game.lowerBound - 1)
            Destroy(gameObject);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (isFriendly && collision.gameObject.CompareTag("Enemy"))
        {
            collision.gameObject.GetComponent<Enemy>().TakeDamage(damage);
            PlayExplosion();
            Destroy(gameObject);
        }

        if (!isFriendly && collision.gameObject.CompareTag("Player"))
        {
            collision.gameObject.GetComponent<PlayerController>().TakeDamage();
            PlayExplosion();
            Destroy(gameObject);
        }
    }

    private void PlayExplosion()
    {
        Vector3 explosionPosition = transform.position;
        GameObject explosion = Instantiate(hitEffect, explosionPosition, transform.rotation);
        AudioSource.PlayClipAtPoint(hitSound, new(0,0,-5));
        explosion.GetComponent<ParticleSystem>().Play();
    }
}
