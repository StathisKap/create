import os
import sys
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC

from secrets import GitHub_Username, GitHub_Password

#open up a new browser window and go to https://github.com/login
chrome_options = Options()  # create a new chrome options object
chrome_options.add_experimental_option("detach", True) #detach the browser from the terminal
chrome_options.add_argument("--log-level=3") #silence the browser
#browser = webdriver.Chrome(service=Service(ChromeDriverManager().install()), options=chrome_options)

browser = webdriver.Chrome("/mnt/c/Users/Stathis Kapnidis/Downloads/chromedriver.exe", options=chrome_options)
browser.get('https://github.com/login')

#if the user is not logged in, login
if browser.find_element(by=By.XPATH, value='//*[@id="login_field"]').is_displayed():
    username = browser.find_element(by=By.XPATH, value='//*[@id="login_field"]')
    password = browser.find_element(by=By.XPATH, value='//*[@id="password"]')
    username.send_keys(GitHub_Username)
    password.send_keys(GitHub_Password)
    browser.find_element(by=By.XPATH, value='//*[@id="login"]/div[4]/form/div/input[12]').click()

##if github is asking for a verification code, wait for the user to enter it 
#if browser.find_element_by_xpath('//*[@id="login"]/p[1]/input').is_displayed(): 
#    verification_code = browser.find_element_by_xpath('//*[@id="login"]/p[1]/input') 
#    verification_code.send_keys("")
#    browser.find_element_by_xpath('//*[@id="login"]/p[2]/input[3]').click()

#go to the New Repository page
browser.get('https://github.com/new')

#create a function that will click on the create new repository button
def create_new_repo():
    browser.execute_script("var scrollingElement = (document.scrollingElement || document.body);scrollingElement.scrollTop = scrollingElement.scrollHeight;")
    WebDriverWait(browser, 10).until(EC.element_to_be_clickable((By.XPATH, "//button[contains(text(),'Creat')]"))).click()
    print("Repository created")

def enter_text(name,xpath_value):
    if browser.find_element(by=By.XPATH, value=xpath_value).is_displayed():
        repo_name = browser.find_element(by=By.XPATH, value=xpath_value)
        repo_name.send_keys(name)

#if 1 argument is provided, store it as repo_name, if a second one is also provided, store that one as description
if len(sys.argv) == 1:
    print("No arguments provided. Please provide a repository name.")
elif len(sys.argv) == 2:
    #Enter the repo name
    enter_text(name = sys.argv[1], xpath_value ='//*[@id="repository_name"]')
    #wait for 3 seconds
    create_new_repo()

elif len(sys.argv) == 3:
    #Enter the repo name
    enter_text(name = sys.argv[1], xpath_value ='//*[@id="repository_name"]')
    #Enter the description
    enter_text(name = sys.argv[2], xpath_value ='//*[@id="repository_description"]')
    create_new_repo()
elif len(sys.argv) > 3:
    print("Too many arguments. Please enter a name for the repository and an optional description.")
    sys.exit()
    
git_remote = browser.find_element(by=By.XPATH,value='//*[@id="empty-setup-clone-url"]').get_attribute('value')
print(git_remote)

browser.quit()