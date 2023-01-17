using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LightningShield : MonoBehaviour
{
    public float returnDamage = 5;

    public AudioClip hitSound;
    public GameObject hitEffect;

    // Damage enemies on colliding with them
    public void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.CompareTag("Enemy"))
        {
            Vector3 explosionPosition = collision.gameObject.transform.position;
            GameObject explosion = Instantiate(hitEffect, explosionPosition, transform.rotation);
            AudioSource.PlayClipAtPoint(hitSound, new(0, 0, -5));
            explosion.GetComponent<ParticleSystem>().Play();

            collision.gameObject.GetComponent<Enemy>().TakeDamage(returnDamage);
        }
    }
}
