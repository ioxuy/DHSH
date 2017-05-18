using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UserConsole.Code
{
    public class CAccount
    {
        public string AccountName { get; set; }

        public string AccountPass { get; set; }

        public string Area { get; set; }

        public int nOrderIndex { get; set; }

        public string PlayerName { get; set; }

        public string LogText { get; set; }

    }
    public class AccountLogic
    {
        public static List<CAccount> GetAccountList()
        {
            var FileList = GetAccountFileContent();
            
            var Lst = new List<CAccount>();
            foreach (var itm in FileList)
            {
                var strArray = itm.Split(new string[] { "," }, StringSplitOptions.RemoveEmptyEntries);
                if (strArray.Length != 6)
                {
                    MessageBox.Show("帐号格式错误:[" + itm + "],主要要用',',而不能用'，'");
                    return Lst;
                }

                int _OrderIndex = -1;
                for (int i = 0; i < 9; i++)
                {
                    if (strArray[3].IndexOf(i.ToString()) != -1)
                    {
                        _OrderIndex = i - 1;
                        break;
                    }
                }

                if (_OrderIndex == -1)
                {
                    MessageBox.Show("角色格式错误:[" + strArray[3] + "]");
                    return Lst;
                }

                Lst.Add(new CAccount() { AccountName = strArray[0], AccountPass = strArray[1], Area = strArray[2], PlayerName = strArray[4], nOrderIndex = _OrderIndex });
            }

            return Lst;
        }

        private static List<String> GetAccountFileContent()
        {
            var Lst = new List<String>();

            using (StreamReader SR = new StreamReader(System.Environment.CurrentDirectory +  @"\Account.txt", Encoding.ASCII))
            {
                String strText = null;
                while ((strText = SR.ReadLine()) != null)
                    Lst.Add(strText);
                
                SR.Close();
            }

            return Lst;
        }
    }
}
