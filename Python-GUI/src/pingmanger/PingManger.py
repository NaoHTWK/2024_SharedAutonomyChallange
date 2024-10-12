import platform
import subprocess

class PingManager:
    def __init__(self, ip_addresses):
        self.ip_addresses = ip_addresses

    def ping_host_linux(self, ip):
        try:
            print("[+] Ping host {}".format(ip), "from linux")
            # Using subprocess to call the system's ping command
            response = subprocess.run(['ping', '-c', '4', ip], capture_output=True, text=True)
            return response.returncode == 0
        except subprocess.TimeoutExpired:
            print(f"Timeout beim Pingen von {ip}")
            return False
        except Exception as e:
            print(f"Fehler beim Pingen von {ip}: {str(e)}")
            return False

    def ping_host_windows(self, ip):
        try:
            print("[+] Ping host {}".format(ip), "from windows")
            from pythonping import ping
            result = ping(ip, count=1, timeout=2)
            return result.success()
        except Exception as e:
            print(f"Fehler beim Pingen von {ip}: {str(e)}")
            return False

    def ping_host(self, ip):
        if platform.system() == "Linux":
            return self.ping_host_linux(ip)
        elif platform.system() == "Windows":
            return self.ping_host_windows(ip)
        else:
            print("Unsupported operating system")
            return False

    def get_reachable_ips(self):
        reachable_ips = []
        for ip in self.ip_addresses:
            if self.ping_host(ip):
                reachable_ips.append(ip)
        print(f"Returned reachable IPs: {reachable_ips}")
        return reachable_ips

