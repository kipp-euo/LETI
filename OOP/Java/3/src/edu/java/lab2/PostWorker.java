package edu.java.lab2;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.table.DefaultTableModel;


/**
 * UI for Post Office Worker application.
 * @author Beznogov A.V.
 * @version 1.0
 */
public class PostWorker {

	private JFrame mainFrame;
	
	private JToolBar toolBar;
	
	private JButton openBtn;
	private JButton saveBtn;
	private JButton addBtn;
	private JButton editBtn;
	private JButton deleteBtn;
	
	private JTable dataTable;
	private DefaultTableModel tableModel;
	private JScrollPane scrollPane;
	
	private JPanel filterPanel;
	private JComboBox<String> viewCombo;
	private JTextField searchField;
	private JButton searchBtn;


	/**
	 * Creates the toolbar for the UI application.
	 * Implements functionality for the Add row (+) and Delete row (trash bin) buttons
	 */
	private void create_toolbar() {
		toolBar = new JToolBar("Панель управления");

		openBtn = new JButton(new ImageIcon("./img/open.png"));
		saveBtn = new JButton(new ImageIcon("./img/save.png"));
		addBtn = new JButton(new ImageIcon("./img/add.png"));
		editBtn = new JButton(new ImageIcon("./img/edit.png"));
		deleteBtn = new JButton(new ImageIcon("./img/delete.png"));

		openBtn.setToolTipText("Открыть файл данных");
		saveBtn.setToolTipText("Сохранить изменения");
		addBtn.setToolTipText("Добавить подписку/клиента");
		editBtn.setToolTipText("Редактировать выбранную запись");
		deleteBtn.setToolTipText("Удалить выбранную запись");
		
		addBtn.addActionListener(e -> {
			String id = String.format("%04d", tableModel.getRowCount() + 1);
			tableModel.addRow(new Object[]{id});
		});

		deleteBtn.addActionListener(e -> {
			tableModel.removeRow(dataTable.getSelectedRow());
		});

		toolBar.add(openBtn);
		toolBar.add(saveBtn);
		toolBar.add(addBtn);
		toolBar.add(editBtn);
		toolBar.add(deleteBtn);

		mainFrame.add(toolBar, BorderLayout.NORTH);
	}

	private void create_table() {
		String[] columns = {"ID", "ФИО Клиента", "Адрес доставки", "Издание", "Почтальон"};
		Object[][] data = {
			{"0001", "Иванов И. И.", "Невский пр., д. 12, кв. 4", "Газета 'СПб ведомости'", "Смирнов А. В."},
			{"0002", "Петров П. С.", "ул. Пушкина, д. 5, кв. 18", "Журнал 'Наука и жизнь'", "Васильева Е. Н."},
			{"0003", "Сидорова А. М.", "Садовая ул., д. 28, кв. 2", "Газета 'АиФ'", "Смирнов А. В."},
			{"0004", "Кузнецов Д. В.", "пер. Гривцова, д. 3, кв. 11", "Журнал 'Вокруг света'", "Николаев К. М."},
			{"0005", "Михайлова О. И.", "ул. Марата, д. 45, кв. 9", "Газета 'Коммерсантъ'", "Васильева Е. Н."}
		};
		tableModel = new DefaultTableModel(data, columns);
		dataTable = new JTable(tableModel);
		scrollPane = new JScrollPane(dataTable);

		mainFrame.add(scrollPane, BorderLayout.CENTER);
	}

	/**
	 * Creates the bottom panel for the UI application
	 * Implements searching clients by name
	 */
	private void create_bottom_panel() {
		filterPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
		viewCombo = new JComboBox<>(new String[]{
			"Подписки клиентов",
			"Номенклатура газет и журналов",
			"Список почтальонов",
			"Отчет о доставке"
		});
		filterPanel.add(viewCombo);
		searchField = new JTextField("Поиск по ФИО или изданию...", 22);
		searchBtn = new JButton("Найти");

		searchBtn.addActionListener(l -> {
			Vector<String> found_ids = new Vector<>();
			String query_name = searchField.getText();
			for (int i = 0; i < tableModel.getRowCount(); i++) {
				String name = (String)tableModel.getValueAt(i, 1);
				if (name.contains(query_name)) {
					found_ids.add((String)tableModel.getValueAt(i, 0));
				}
			}
			JOptionPane.showMessageDialog(mainFrame, "ID клиентов с подходящим ФИО:\n" + String.join(", ", found_ids));
		});

		filterPanel.add(searchField);
		filterPanel.add(searchBtn);

		mainFrame.add(filterPanel, BorderLayout.SOUTH);
	}

	public void show() {
		mainFrame = new JFrame("Работник почты");
		mainFrame.setSize(850, 500);
		mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		mainFrame.setLayout(new BorderLayout());
		
		create_toolbar();
		create_table();
		create_bottom_panel();

		mainFrame.setVisible(true);
	}

	public static void main(String[] args) {
		new PostWorker().show();
	}
}
