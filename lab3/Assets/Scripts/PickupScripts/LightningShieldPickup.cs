using System;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class LightningShieldPickup : Powerup
{
    public float returnDmg = 5;
    public float buffDuration = 8.0f;
    public int buffId = 0;

    public AudioClip pickupSound;
    public AudioClip expireSound;
    public AudioClip hitSound;
    public GameObject hitEffect;

    private ActiveBuffIndicator indicator;

    public override void OnPickup()
    {
        AudioSource.PlayClipAtPoint(pickupSound, new(0, 0, -5));
        player.GetComponent<PlayerController>().ApplyBuff(buffId, ShieldBuffCo());
    }

    private IEnumerator ShieldBuffCo()
    {
        if (!player.GetComponent<LightningShield>())
            player.AddComponent<LightningShield>();

        LightningShield shield = player.GetComponent<LightningShield>();
        shield.returnDamage = returnDmg;
        shield.hitEffect = hitEffect;
        shield.hitSound = hitSound;

        player.GetComponent<PlayerController>().invulnerable = true;

        yield return new WaitForSeconds(buffDuration);

        Destroy(shield);
        player.GetComponent<PlayerController>().invulnerable = false;
        AudioSource.PlayClipAtPoint(expireSound, new(0, 0, -5));

        player.GetComponent<PlayerController>().RemoveBuff(buffId);
    }
}
