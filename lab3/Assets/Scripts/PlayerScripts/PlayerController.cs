using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using static System.Math;

public class PlayerController : MonoBehaviour
{

    [Header("Resources")]
    private float speed = 10;
    private float maxMana = 20;
    private float manaRegen = 1.5f;
    private int maxHP = 3;

    private Vector3 startingPosition = new Vector3(-10, 0, 0);

    [Header("Projectile")]
    public float spellDamageModifier = 1.0f;
    public float spellDamageFlatBonus = 0.0f;
    public GameObject projectilePrefab;

    [Header("Resources")]
    public GameObject heartContainers;
    private HeartManager hp;
    public GameObject manaBar;
    private ResourceBarManager mana;
    public GameObject buffIndicator;
    private ActiveBuffIndicator buffs;

    public GameManager game;
    public GameOverMenu gameOverMenu;

    [Header("Audio")]
    private AudioSource playerAudio;
    public AudioClip hurtSound;

    [Header("IFrames")]
    public Color flashColor;
    public Color regularColor;
    public float flashDuration;
    public int numOfFlashes;
    public Collider2D triggerCollider;
    public SpriteRenderer playerSprite;
    public bool invulnerable = false;

    // Start is called before the first frame update
    void Start()
    {
        game = GameObject.FindGameObjectWithTag("GameManager").GetComponent<GameManager>();

        hp = heartContainers.GetComponent<HeartManager>();
        mana = manaBar.GetComponent<ResourceBarManager>();
        playerAudio = GetComponent<AudioSource>();
        buffs = buffIndicator.GetComponent<ActiveBuffIndicator>();

        hp.SetMaxHp(maxHP);
        mana.SetMaxValue(maxMana);
    }

    void Update()
    {
        MovePlayer();
        ConstrainPlayerPosition();
        CastSpell();
        ReplenishMana(Time.deltaTime * manaRegen);
    }


    // Move the player based on user input
    void MovePlayer()
    {
        if (!game.paused) {
            float horizontalInput = Input.GetAxis("Horizontal");
            float verticalInput = Input.GetAxis("Vertical");
            transform.Translate(new Vector3(horizontalInput * Time.deltaTime * speed, verticalInput * Time.deltaTime * speed, 0));
        }
    }

    // Check if the player left the view boundaries
    void ConstrainPlayerPosition()
    {
        if (transform.position.y > game.upperBound)
            transform.position = new Vector3(transform.position.x, game.upperBound, 0);
        else if (transform.position.y < game.lowerBound)
            transform.position = new Vector3(transform.position.x, game.lowerBound, 0);

        if (transform.position.x > game.rightBound)
            transform.position = new Vector3(game.rightBound, transform.position.y, 0);
        else if (transform.position.x < game.leftBound)
            transform.position = new Vector3(game.leftBound, transform.position.y, 0);
    }

    void CastSpell()
    {
        if (!game.paused && Input.GetMouseButtonDown(0)) 
        {
            if (mana.GetValue() < 2) 
            {
                Debug.Log("Not enough mana");
                return; 
            }

            mana.ModifyValue(-2);

            Vector3 mousePos = Input.mousePosition;
            mousePos.z = Camera.main.nearClipPlane;
            Vector3 dest = Camera.main.ScreenToWorldPoint(mousePos);
            Vector3 playerPos = transform.position;

            Vector2 direction = (Vector2) (dest - playerPos);
            direction.Normalize();

            Vector3 spawnPos = new Vector3(transform.position.x, transform.position.y, 0);
            Quaternion rotation = Quaternion.FromToRotation(Vector2.down, direction);
            GameObject projectile = Instantiate(projectilePrefab, spawnPos, rotation);
            
            ProjectileManager pManager = projectile.GetComponent<ProjectileManager>();
            pManager.direction = direction;
            pManager.isFriendly = true;

            float baseDmg = pManager.damage;
            pManager.damage = CalcFinalDamage(baseDmg);
        }
    }
    public void TakeDamage()
    {
        if (!invulnerable)
        {
            hp.RemoveHearts(1);
            playerAudio.PlayOneShot(hurtSound);
            StartCoroutine(FlashCo());
        }

        if (hp.hp <= 0) GameOver();
    }

    public void ReplenishHp(int amount)
    {
        hp.AddHearts(amount);
    }

    public void ReplenishMana(float amount)
    {
        mana.ModifyValue(amount);
    }

    private IEnumerator FlashCo()
    {
        int flashCount = 0;
        invulnerable = true;
        Color originalColor = playerSprite.color;
        while (flashCount < numOfFlashes)
        {
            playerSprite.color = flashColor;
            yield return new WaitForSeconds(flashDuration);
            playerSprite.color = regularColor;
            yield return new WaitForSeconds(flashDuration);
            flashCount++;
        }
        playerSprite.color = originalColor;
        invulnerable = false;
    }

    public void ApplyBuff(int buffId, IEnumerator buffCo)
    {
        // If buff is already active, deactivate it first to refresh duration
        if (buffs.activeBuffs[buffId])
        {
            RemoveBuff(buffId);
        }

        buffs.SetBuffActive(true, buffId);
        StartCoroutine(buffCo);
    }

    public void RemoveBuff(int buffId)
    {
        buffs.SetBuffActive(false, buffId);
    }

    private float CalcFinalDamage(float baseDmg)
    {
        return (baseDmg + spellDamageFlatBonus) * spellDamageModifier;
    }

    private void GameOver()
    {
        game.gameOver = true;
        gameOverMenu.Setup(game.score);
    }
}   
