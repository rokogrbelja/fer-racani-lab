using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ManaPickup : Powerup
{
    public float maxMana = 10;
    public float minMana = 5;

    public AudioClip pickupSound;

    public override void OnPickup()
    {
        float mana  = Random.Range(minMana, maxMana);
        player.GetComponent<PlayerController>().ReplenishMana(mana);
        AudioSource.PlayClipAtPoint(pickupSound, new(0, 0, -5));
    }
}
