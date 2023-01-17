using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class GameOverMenu : MonoBehaviour
{
    public TextMeshProUGUI finalScoreText;
    public GameManager game;

    public void Setup(float finalScore)
    {
        game.paused = true;
        Time.timeScale = 0;
        gameObject.SetActive(true);
        finalScoreText.text = finalScore.ToString();
    }

    public void RestartGame()
    {
        game.ResumeGame();
        game.gameOver = false;
        SceneManager.LoadScene(1);
    }

    public void MainMenu()
    {
        game.ResumeGame();
        SceneManager.LoadScene(0);
    }
}
