using System.Collections;
using UnityEngine;

public class DmgUpPickup : Powerup
{
    public float dmgModifier = 2.0f;
    public float buffDuration = 8.0f;
    public int buffId = 1;

    public AudioClip pickupSound;
    public AudioClip expireSound;

    public override void OnPickup()
    {
        AudioSource.PlayClipAtPoint(pickupSound, new(0, 0, -5));
        player.GetComponent<PlayerController>().ApplyBuff(buffId, DmgBuffCo());
    }

    private IEnumerator DmgBuffCo()
    {
        player.GetComponent<PlayerController>().spellDamageFlatBonus += 5;

        yield return new WaitForSeconds(buffDuration);

        player.GetComponent<PlayerController>().spellDamageFlatBonus -= 5;
        AudioSource.PlayClipAtPoint(expireSound, new(0, 0, -5));
        player.GetComponent<PlayerController>().RemoveBuff(buffId);
    }
}
