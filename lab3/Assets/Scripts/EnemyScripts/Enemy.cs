using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy : MonoBehaviour
{

    public float hp = 10;
    public float speed = 10.0f;
    public float sightRange = 0.0f;
    public float attackRange = 0.0f;
    public int scoreValue = 10;

    public AudioClip attackSound;

    [SerializeField]
    private EnemyData data;

    public GameObject player;
    public GameManager game;

    private EnemySpawnManager spawnManager;

    protected Animator anim;

    // Start is called before the first frame update
    void Start()
    {
        hp = data.hp; 
        speed = data.speed; 
        sightRange = data.sightRange;
        attackRange = data.attackRange;
        attackSound = data.attackSound;
        scoreValue = data.scoreValue;

        player = GameObject.FindGameObjectWithTag("Player");
        game = GameObject.FindGameObjectWithTag("GameManager").GetComponent<GameManager>();
        spawnManager = GameObject.FindGameObjectWithTag("EnemySpawner").GetComponent<EnemySpawnManager>();
        anim = GetComponent<Animator>();

        InitEnemy();
    }

    // Update is called once per frame
    void Update()
    {
        Move();
        FacePlayer();
        Attack();
        DestroyOutOfBounds();
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Player"))
        {
            player.GetComponent<PlayerController>().TakeDamage();
        }
    }

    private void DestroyOutOfBounds()
    {
        // Destroy enemies that leave the view
        if (transform.position.x < game.leftBound - 3) Destroy(gameObject);

        if (transform.position.y > game.upperBound + 1)
            transform.position = new(transform.position.x, game.upperBound, transform.position.z);

        if(transform.position.y < game.lowerBound - 1)
            transform.position = new(transform.position.x, game.lowerBound, transform.position.z);
    }

    public bool PlayerWithinRange(float range)
    {
        return Vector3.Distance(player.transform.position, transform.position) <= range;
    }

    public void TakeDamage(float amount)
    {
        hp -= amount;
        if (hp <= 0) Die();
    }

    public virtual void InitEnemy() { }

    // Enemies have different locations at which day spawn
    // By default they spawn in a randomly selected location on the y-axis outside of the camera view on the left side of the screen
    public virtual void Spawn()
    {
        // Randomly choose a location in the defined range
        float spawnPosY = Random.Range(game.lowerBound, game.upperBound);
        Vector3 spawnPos = new(game.rightBound+3, spawnPosY, 0);

        Instantiate(gameObject, spawnPos, transform.rotation);
    }

    // Enemies have different patterns of movement which have to be defined for each enemy type
    // If not defined differently, the enemy moves left using its speed
    public virtual void Move()
    {
        transform.Translate(Vector3.left * speed * Time.deltaTime);
    }

    // Flip the sprite so that the enemy faces the player if within sight range
    public virtual void FacePlayer()
    {
        if (PlayerWithinRange(sightRange))
        {
            if (player.transform.position.x > transform.position.x)
            {
                GetComponent<SpriteRenderer>().flipX = true;
            } else
            {
                GetComponent<SpriteRenderer>().flipX = false;
            }
        }
        else
        {
            GetComponent<SpriteRenderer>().flipX = false;
        }
    }

    // Enemies have different attack patterns which have to be defined for each enemy type
    public virtual void Attack()
    {
    }

    public virtual void Die()
    {
        game.AddScore(scoreValue);
        Destroy(gameObject);
    }

}
