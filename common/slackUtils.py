#!/usr/bin/env python3

import collections
import string
import platform
import subprocess
import re
import os.path
import datetime
import json
import requests

def slackStatusPost(message):
    """
    Post a status update to slack
    """

    #Based on work on https://gist.github.com/devStepsize/b1b795309a217d24566dcc0ad136f784

    try:
        webhook_url = os.environ["SLACK_API_URL"]

        slack_data = {'text': message}

        response = requests.post(
            webhook_url, data=json.dumps(slack_data),
            headers={'Content-Type': 'application/json'}
        )

        if response.status_code != 200:
            print('Could not post slack message, code ' + str(response.status_code) + ' ... continuing')
    except KeyError:
        print('Could not read slack API url')
    except requests.exceptions.ConnectionError:
        print('Slack connection error ... continuing')