using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PauseMenu : MonoBehaviour
{
    public GameManager game;
    public void Open()
    {
        gameObject.SetActive(true);
    }

    public void Close()
    {
        gameObject.SetActive(false);
    }

    public void ResumeGame()
    {
        game.paused = false;
        Time.timeScale = 1.0f;
        Close();
    }

    public void MainMenu()
    {
        game.ResumeGame();
        SceneManager.LoadScene(0);
    }
}
