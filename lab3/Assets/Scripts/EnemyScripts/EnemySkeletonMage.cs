using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemySkeletonMage : Enemy
{
    private bool attacking;
    private bool projectileFired;
    private float attackDuration;
    private float attackTimer;
    private float attackCooldown;
    private float attackCooldownTimer;
    public GameObject projectilePrefab;

    public override void InitEnemy()
    {
        // Put Skeleton on the ground once spawned
        transform.position = new Vector3(transform.position.x, game.lowerBound, transform.position.z);
        attacking = false;
        projectileFired = false;
        attackDuration = 1f;
        attackCooldown = 1f;
        attackCooldownTimer = 0;
    }

    public override void Attack()
    {
        if (!attacking)
        {
            if (attackCooldownTimer >= attackCooldown)
            {
                attacking = true;
                projectileFired = false;
                attackTimer = 0;
                anim.SetBool("attacking", true);
            }
            attackCooldownTimer += Time.deltaTime;
        }
        if (attacking)
        {
            // Shoot projectile halfway through animation
            if (attackTimer >= attackDuration / 2 && !projectileFired)
            {
                ShootProjectile();
                AudioSource.PlayClipAtPoint(attackSound, new(0, 0, -5));
                projectileFired = true;
            }
            else if (attackTimer >= attackDuration)
            {
                attacking = false;
                attackCooldownTimer = 0;
                anim.SetBool("attacking", false);
            }
            attackTimer += Time.deltaTime;
        }
    }

    public override void Move()
    {
        if (!attacking) 
        {
            // follow the player along the X-axis
            transform.position = Vector2.MoveTowards(transform.position, player.transform.position, Time.deltaTime * speed);
            transform.position = new Vector3(transform.position.x, game.lowerBound, transform.position.z);

        }
    }

    private void ShootProjectile()
    {
        Vector3 dest = player.transform.position;
        Vector3 origin = transform.position;

        Vector2 direction = (Vector2)(dest - origin);
        direction.Normalize();

        Vector3 spawnPos = new Vector3(transform.position.x, transform.position.y, 0);
        Quaternion rotation = Quaternion.FromToRotation(Vector2.down, direction);
        GameObject projectile = Instantiate(projectilePrefab, spawnPos, rotation);

        projectile.GetComponent<ProjectileManager>().direction = direction;
        projectile.GetComponent<ProjectileManager>().isFriendly = false;
    }
}
