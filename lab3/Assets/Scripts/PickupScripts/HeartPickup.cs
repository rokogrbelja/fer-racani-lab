using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HeartPickup : Powerup
{
    public int hp = 1;

    public AudioClip pickupSound;

    public override void OnPickup()
    {
        player.GetComponent<PlayerController>().ReplenishHp(hp);
        AudioSource.PlayClipAtPoint(pickupSound, new(0, 0, -5));
    }
}
