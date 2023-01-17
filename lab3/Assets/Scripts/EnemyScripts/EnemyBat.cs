using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyBat : Enemy
{
    private bool windingUp;
    private bool charging;
    private float windUpTime;
    private float windUpTimePassed;
    private float chargingTime;
    private float chargingTimePassed;
    private Vector3 chargeDirection;

    // Called immediately upon spawning enemy
    public override void InitEnemy()
    {
        windingUp = false;
        charging = false;
        windUpTime = 0.5f;
        windUpTimePassed = 0;
        chargingTime = 0.3f;
        chargingTimePassed = 0;
    }

    // The bat follows the player if they are within its sight range
    public override void Move()
    {
        if (!windingUp && !charging) {
            if (PlayerWithinRange(sightRange) && !PlayerWithinRange(attackRange))
                transform.position = Vector2.MoveTowards(transform.position, player.transform.position, Time.deltaTime * speed);
            else
                transform.Translate(Vector3.left * Time.deltaTime * speed);
        }
    }

    // When the bat gets close enough, it charges towards the player a set distance
    public override void Attack()
    {
        // When the bat gets within attack range of the player, it starts winding up its charge
        if (PlayerWithinRange(attackRange))
        {
            if (!windingUp && !charging)
            {
                windingUp = true;
                windUpTimePassed = 0;
            }
        }

        // Stay still for a set amount of time while winding up the charge
        if (windingUp)
        {
            windUpTimePassed += Time.deltaTime;
            if (windUpTimePassed >= windUpTime)
            {
                windUpTimePassed = 0;
                windingUp = false;
                charging = true;
                chargeDirection = player.transform.position - transform.position;
                chargeDirection.Normalize();
                AudioSource.PlayClipAtPoint(attackSound, new(0, 0, -10));
                anim.SetBool("charging", true);
            }
        }

        // Move quickly towards the charging destination for a set amount of time
        if (charging)
        {
            transform.Translate(chargeDirection * Time.deltaTime * speed * 3);
            chargingTimePassed += Time.deltaTime;
            if (chargingTimePassed >= chargingTime)
            {
                chargingTimePassed = 0;
                charging = false;
                anim.SetBool("charging", false);
            }
        }
    }
}
