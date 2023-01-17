using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{
    // Define the boundaries of the camera's view
    public float leftBound = -17;
    public float rightBound = 17;
    public float upperBound = 9.5f;
    public float lowerBound = -6f;

    public int score = 0;
    public Text scoreCounter;

    public float timeElapsed = 0;
    public float difficulty = 0;

    public PauseMenu pauseMenu;
    public bool paused = false;
    public bool gameOver = false;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        timeElapsed += Time.deltaTime;
        scoreCounter.text = "Score: " + score;
        HandlePlayerInput();
    }

    public void AddScore(int amount)
    {
        score += amount;
    }

    public void HandlePlayerInput()
    {
        if (!gameOver && Input.GetKeyDown(KeyCode.Escape))
        {
            if (!paused) PauseGame();
            else ResumeGame();
        }
    }

    public void PauseGame()
    {
        paused = true;
        Time.timeScale = 0;
        pauseMenu.Open();
    }

    public void ResumeGame()
    {
        paused = false;
        Time.timeScale = 1;
        pauseMenu.Close();
    }
}
